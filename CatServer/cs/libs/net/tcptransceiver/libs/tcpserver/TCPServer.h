#pragma once

#include "TCPAcceptor.h"
#include "TCPStream.h"
#include <memory>

class TCPServer: public TCPAcceptor {
    private:
        int sd;
        std::string address;
        int port;
        bool listening;
    public:
        int        start() override;
        std::unique_ptr<TCPStream> accept() override;
        TCPServer(int port_, const char * address_ = ""): port(port_), address(address_), listening(false) {};
        ~TCPServer();


};
