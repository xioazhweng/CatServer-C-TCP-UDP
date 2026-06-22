#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>


class UDPTransceiverI {
    public:
        virtual ssize_t send(const sockaddr_in & addr, const char * buffer, size_t len) = 0;
        virtual std::string & getIp() = 0;
        virtual uint32_t getPort() = 0;
        virtual ssize_t receive(sockaddr_in & addr, char * buffer, size_t len) = 0;
        virtual ~UDPTransceiverI() = default;
};     