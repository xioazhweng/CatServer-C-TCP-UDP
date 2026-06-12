#include "TCPServer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "../tcpstream/TCPStreamImpl.h"
#include <cstring> 

TCPServer::~TCPServer() {
    if (sd > 0) close(sd);
};

int TCPServer::start() {
    if (listening) return 0;
    socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    if (address.size() > 0) {
        inet_pton(PF_INET, address.c_str(), &(addr.sin_addr));
    } else {
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    int optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    int result = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (result == 0) listening = true;
    return result;
}

TCPStream * TCPServer::accept() {
    if (!listening) return nullptr;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    int sd = ::accept(sd, (struct sockaddr*)&addr, &len);
    if (sd < 0) return nullptr;
     TCPStreamImpl * stream;
    try {
        stream = new TCPStreamImpl(sd, &addr);
    } catch (...) {
        close(sd);
    }
    return stream;
}