
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "TCPClient.h"
#include "../tcpstream/TCPStreamImpl.h"

 TCPStream * TCPClient::connect(int port_, const char * server) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    if (resolveHostName(server, &(addr.sin_addr)) != 0) {
        inet_pton(PF_INET, server, &(addr.sin_addr));
    }
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (::connect(sd, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        close(sd);
        return nullptr;
    }
    TCPStreamImpl * stream = nullptr;
    try {
        stream = new TCPStreamImpl(sd, &addr);
    } catch (...) {
        close(sd);
    }
    return stream;
 }

int TCPClient::resolveHostName(const char* host, struct in_addr* addr) {
    return -1;
};