#pragma once
#include <netinet/in.h>
#include "tcpstream/TCPStream.h"

class TCPConnector {
    public:
        virtual TCPStream * connect(int port_, const char * server) = 0;
        virtual ~TCPConnector()  = default;
    
};