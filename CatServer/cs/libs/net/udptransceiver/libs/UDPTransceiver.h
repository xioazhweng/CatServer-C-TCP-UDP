#pragma once 

#include "../include/UDPTransceiverI.h"

class UDPTransceiver: public UDPTransceiverI{
    public:
        UDPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_);
        ssize_t send(const sockaddr_in & addr, const char * buffer, size_t len) override;
        std::string & getIp() override {return ip;};
        uint32_t getPort() override {return port;};
        ssize_t receive(sockaddr_in & addr, char * buffer, size_t len) override;
        ~UDPTransceiver();
    private:
        int sd;
        std::string ip;
        uint32_t port;
        void create_socket();
        void close_socket();
};