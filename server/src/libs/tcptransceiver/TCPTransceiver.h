#pragma once


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class TCPTransceiver {
    private:
        const size_t buff_size = 4096;
        std::pair<uint32_t, std::string> server;
        sockaddr_in server_addr;
        int server_sock;
        int client_sock;
        void create_socket();
        void close_socket();
        struct sockaddr_in  get_addr(uint32_t port, const std::string & ip_addr);
        struct sockaddr_in  get_addr(const std::pair<uint32_t, std::string> & addr);
    public:
        TCPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_);
        std::pair<uint32_t, std::string> get_netaddr() const;
        void connect(const std::pair<uint32_t, std::string> & addr);
        void connect();
        void close_connection();
        void send_msg(const std::string & msg);
        std::string receive_msg();
        ~TCPTransceiver() ;
};     