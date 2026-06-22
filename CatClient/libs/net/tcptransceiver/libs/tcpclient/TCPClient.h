#pragma once


#include "TCPConnector.h" 
#include "TCPStream.h"

class TCPClient: public TCPConnector {
    public:
        TCPClient() = default;
        std::unique_ptr<TCPStream> connect(int port_, const char * server) override;
        ~TCPClient() = default;
    private:
        int resolveHostName(const char* host, struct in_addr* addr);
        
};  
