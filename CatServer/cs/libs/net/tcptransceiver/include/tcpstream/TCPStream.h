#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
class TCPStream {
    public:

        virtual ssize_t send(const char * buffer, size_t len) = 0;
        virtual ssize_t receive(char* buffer, size_t len) = 0;
        virtual std::string getPeerIP() = 0;
        virtual int getPeerPort() = 0;

        virtual ~TCPStream() = default;
};