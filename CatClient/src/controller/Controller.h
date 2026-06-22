#pragma once
#include <string>
#include <queue>
#include <deque>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

#include "UDPTransceiverI.h"
#include "UDPTransceiver.h"
#include "TCPStream.h"
#include "TCPClient.h"

enum MSGType { TCP, UDP, NONE };

const size_t buffer_size = 8096;

// Резолвит доменное имя или IP в sockaddr_in
inline sockaddr_in resolve(const std::string & host, uint32_t port) {
    struct addrinfo hints{}, * res;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0 || !res)
        throw std::runtime_error("Cannot resolve: " + host);
    sockaddr_in addr = *reinterpret_cast<sockaddr_in*>(res->ai_addr);
    addr.sin_port = htons(port);
    freeaddrinfo(res);
    return addr;
}

class NetworkController {
public:
    NetworkController(const std::string & client_ip,  uint32_t client_udp_port,
                      const std::string & server_host, uint32_t server_udp_port,
                      uint32_t server_tcp_port) {
        // UDP: биндимся на клиентский порт, адрес сервера резолвим
        udp_server_addr_ = resolve(server_host, server_udp_port);
        udp_client_ = std::make_unique<UDPTransceiver>(client_udp_port, client_ip);

        // TCP: коннектимся по имени/IP на tcp-порт
        tcp_client_ = std::make_unique<TCPClient>();
        tcp_stream_ = tcp_client_->connect(server_tcp_port, server_host.c_str());
    }

    void send(const std::pair<MSGType, std::string> & unit);
    void listen();
    void stop_listening() { listening_ = false; }

    bool is_msg_queue_empty() {
        std::lock_guard<std::mutex> lk(que_mutex_);
        return que_.empty();
    }
    bool back_queue(std::pair<MSGType, std::string> & out);
    bool back_logs(std::string & out);

    ~NetworkController() {
        listening_ = false;
        if (udp_thread_.joinable()) udp_thread_.join();
        if (tcp_thread_.joinable()) tcp_thread_.join();
        delete[] buffer_;
    }

private:
    sockaddr_in                      udp_server_addr_{};
    std::unique_ptr<UDPTransceiverI> udp_client_;
    std::unique_ptr<TCPClient>       tcp_client_;
    std::unique_ptr<TCPStream>       tcp_stream_ = nullptr;

    char * buffer_ = new char[buffer_size];

    std::atomic<bool> listening_{false};
    std::thread udp_thread_;
    std::thread tcp_thread_;

    std::mutex que_mutex_;
    std::mutex log_mutex_;
    std::queue<std::pair<MSGType, std::string>> que_;
    std::deque<std::string> logs_;

    void listen_udp();
    void listen_tcp();
};
