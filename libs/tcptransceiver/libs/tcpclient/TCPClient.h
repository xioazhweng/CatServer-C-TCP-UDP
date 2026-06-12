#pragma once


#include "tcpconnector/TCPConnector.h" 
#include "tcpstream/TCPStream.h"

class TCPClient: public TCPConnector {
    public:
        TCPClient() = default;
        TCPStream * connect(int port_, const char * server) override;
        ~TCPClient() = default;
    private:
        int resolveHostName(const char* host, struct in_addr* addr);
        
};  
