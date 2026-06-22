#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "TCPClient.h"
#include "TCPStreamImpl.h"

std::unique_ptr<TCPStream> TCPClient::connect(int port_, const char * server) {
    // getaddrinfo резолвит и IP и доменные имена
    struct addrinfo hints{}, * res;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(server, nullptr, &hints, &res) != 0 || !res)
        return nullptr;

    struct sockaddr_in addr = *reinterpret_cast<sockaddr_in*>(res->ai_addr);
    addr.sin_port = htons(port_);
    freeaddrinfo(res);

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (::connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(sd);
        return nullptr;
    }

    try {
        return std::unique_ptr<TCPStreamImpl>(new TCPStreamImpl(sd, &addr));
    } catch (...) {
        close(sd);
        return nullptr;
    }
}
