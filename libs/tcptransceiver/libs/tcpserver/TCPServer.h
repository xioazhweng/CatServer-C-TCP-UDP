#pragma once

#include "tcpacceptor/TCPAcceptor.h"
#include "tcpstream/TCPStream.h"
class TCPServer: public TCPAcceptor {
    private:
        int sd;
        std::string address;
        int port;
        bool listening;
    public:
        int        start() override;
        TCPStream* accept() override;
        TCPServer(int port_, const char * address_ = ""): port(port_), address(address_), listening(false) {};
        ~TCPServer();


};
