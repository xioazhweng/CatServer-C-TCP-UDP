#pragma once
#include <string>
#include <queue>
#include <deque>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <cstring>

#include <tcpconnector/TCPConnector.h>
#include "../transceivers/udptransceiver/include/UDPTransceiverI.h"

#include "../transceivers/tcptransceiver/include/tcpstream/TCPStream.h"
#include "../transceivers/udptransceiver/libs/UDPTransceiver.h"
#include "../transceivers/tcptransceiver/libs/tcpclient/TCPClient.h"

enum MSGType {
    TCP,
    UDP,
    NONE
};

const size_t buffer_size = 8096;

class NetworkController {
    private:
        std::mutex que_mutex;
        std::mutex log_mutex;
        std::atomic<bool> listening = {false};
        std::thread udp_thread;
        std::thread tcp_thread;
        char * buffer = new char[buffer_size];
       
        sockaddr_in server_addr;
        sockaddr_in client_addr;
        
        std::queue<std::pair<MSGType, std::string>> que;
        std::deque<std::string> logs;
        std::queue<std::string> udp_que;
        std::unique_ptr<UDPTransceiverI> udp_client;
        std::unique_ptr<TCPClient> tcp_client;
        TCPStream * tcp_stream;
      
        void listen_udp();
        void listen_tcp();
    public:
        NetworkController(uint32_t src_port, const std::string & src_ip, 
                          uint32_t dst_port, const std::string & dst_ip) {
            memset(&server_addr, 0, sizeof(server_addr));
            memset(&client_addr, 0, sizeof(client_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(dst_port);
            inet_pton(AF_INET, dst_ip.c_str(), &server_addr.sin_addr);   
            
            client_addr.sin_family = AF_INET;
            client_addr.sin_port = htons(src_port);
            inet_pton(AF_INET, src_ip.c_str(), &client_addr.sin_addr);  
            
            udp_client = std::make_unique<UDPTransceiver>(src_port, src_ip);
            tcp_client = std::make_unique<TCPClient>();
            tcp_stream = tcp_client->connect(src_port, src_ip.c_str());
            if (tcp_stream == nullptr) {
                throw std::runtime_error("Can't establesh TCP connection");
            }
        };
        
        void send(const std::pair<MSGType, std::string> & unit);
  
        bool is_msg_queue_empty() {
             std::lock_guard<std::mutex> lock(que_mutex);
            return que.empty();
        };
        bool is_logs_queue_empty() {
            std::lock_guard<std::mutex> lock(log_mutex);
            return logs.empty();
        };
        bool back_queue(std::pair<MSGType, std::string> & out);
        bool back_logs(std::string & out);
        void listen();
        void stop_listening() {listening = false;};
        ~NetworkController() {
            listening = false;
            udp_thread.join();
            tcp_thread.join();
        }
        
};