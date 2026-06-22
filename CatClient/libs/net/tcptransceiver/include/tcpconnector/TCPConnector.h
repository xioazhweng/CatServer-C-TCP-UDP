#pragma once
#include <netinet/in.h>
#include "TCPStream.h"
#include <memory>

class TCPConnector {
    public:
        virtual std::unique_ptr<TCPStream> connect(int port_, const char * server) = 0;
        virtual ~TCPConnector()  = default;
    
};