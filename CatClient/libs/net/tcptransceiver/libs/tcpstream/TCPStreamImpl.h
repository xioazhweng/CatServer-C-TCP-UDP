#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "TCPStream.h"
#include "TCPClient.h"

class TCPStreamImpl: public TCPStream {
    public:
        friend class TCPClient;
        ssize_t send(const char * buffer, size_t len) override;
        ssize_t receive(char* buffer, size_t len) override;
        std::string getPeerIP() override { return peer_ip; }
        int getPeerPort()       override { return peer_port; }
        ~TCPStreamImpl() = default;
    private:
        int sd;
        std::string peer_ip;
        int peer_port;
        TCPStreamImpl(int sd_, struct sockaddr_in * addr)
            : sd(sd_), peer_port(ntohs(addr->sin_port)) {
            char ip[50];
            inet_ntop(PF_INET, &addr->sin_addr.s_addr, ip, sizeof(ip)-1);
            peer_ip = ip;
        }
        TCPStreamImpl(const TCPStreamImpl &) = default;
};
