#pragma once

#include "tcpstream/TCPStream.h"

class TCPAcceptor {
    public:
        virtual int        start() = 0;
        virtual TCPStream* accept() = 0;
        virtual ~TCPAcceptor() = default;
};