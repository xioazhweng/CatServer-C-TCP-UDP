#include "TCPStreamImpl.h"

ssize_t TCPStreamImpl::send(const char * buffer, size_t len) {
    return ::send(sd, buffer, len, 0);
};

ssize_t TCPStreamImpl::receive(char* buffer, size_t len) {
    return ::recv(sd, buffer, len, 0);
};