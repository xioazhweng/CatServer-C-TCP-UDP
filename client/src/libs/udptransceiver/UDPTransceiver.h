#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct DataGram {
    std::pair<uint32_t, std::string> addr_;
    std::string msg_;
    DataGram(const std::pair<uint32_t, std::string> & addr, const std::string & msg): msg_(msg), addr_(addr) {};
    DataGram(const sockaddr_in & addr, const std::string& msg): msg_(msg) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,
                  &addr.sin_addr,
                  ip,
                  INET_ADDRSTRLEN);
        uint32_t port = ntohs(addr.sin_port);
        addr_ = {port, ip};
    }
};

class UDPTransceiver {
    private:
        const size_t buff_size = 4096;
        std::pair<uint32_t, std::string> server;
        sockaddr_in server_addr;
        int sock;
        void create_socket();
        void close_socket();
        struct sockaddr_in  get_addr(uint32_t port, const std::string & ip_addr);
        struct sockaddr_in  get_addr(uint32_t port, uint32_t ip_addr);
        struct sockaddr_in  get_addr(const std::pair<uint32_t, std::string> & addr);
    public:
        UDPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_);
        std::pair<uint32_t, std::string> get_netaddr() const;
        void send_msg(const std::pair<uint32_t, std::string> & addr, const std::string & msg);
        void send_msg(const DataGram & dg);
        DataGram receive_msg();
        ~UDPTransceiver() ;
};     