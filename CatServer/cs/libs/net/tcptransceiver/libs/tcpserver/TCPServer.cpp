#include "TCPServer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "TCPStreamImpl.h"
#include <cstring> 
#include <memory>

TCPServer::~TCPServer() {
    if (sd > 0) close(sd);
};

int TCPServer::start() {
    if (listening) return 0;
    sd = socket(PF_INET, SOCK_STREAM, 0);
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
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0) return -1;
    if (::listen(sd, SOMAXCONN) != 0) return -1;
    listening = true;
    return 0;
}

std::unique_ptr<TCPStream> TCPServer::accept() {
    if (!listening) return nullptr;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    int client_sd = ::accept(sd, (struct sockaddr*)&addr, &len);
    if (client_sd < 0) return nullptr;
    std::unique_ptr<TCPStreamImpl> stream;
    try {
        stream = std::unique_ptr<TCPStreamImpl>(new TCPStreamImpl(client_sd, &addr));
    } catch (...) {
        close(sd);
    }
    return stream;
}