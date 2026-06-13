#include <iostream>
#include "TCPServer.h"
#include "TCPStreamImpl.h"
int main(void) {
    TCPServer tcp_server(54434, "192.168.1.4");
    tcp_server.start();
    TCPStream * stream = tcp_server.accept();
    if (stream != nullptr) {
        std::string msh = "HELLO";
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(53443);
        inet_pton(AF_INET, "192.168.1.109", &addr.sin_addr);
        stream->send(msh.c_str(), msh.size());
    }
    return 0;
}