#pragma once

#include "TCPStream.h"
#include <memory>

class TCPAcceptor {
    public:
        virtual int start() = 0;
        virtual std::unique_ptr<TCPStream> accept() = 0;
        virtual ~TCPAcceptor() = default;
};