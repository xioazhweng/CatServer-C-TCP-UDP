#include "TCPTransceiver.h"
#include <stdexcept>
#define LOG(a) std::string("TCPTransceiver: ") + a
TCPTransceiver::TCPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_): 
    server({server_port_, server_ip_addr_}), client_sock(-1) {
    create_socket();
    server_addr = get_addr(server);
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Bind Error");
    };
}

TCPTransceiver::~TCPTransceiver() {
    close_socket();
};

void TCPTransceiver::connect() {
    if (listen(server_sock, 1) < 0) {
        throw std::runtime_error(LOG("connect: listen failed"));
    };
    client_sock = -1;
    while (client_sock < 0) {
        client_sock = accept(server_sock, NULL, NULL);
    }
};
void TCPTransceiver::connect(const std::pair<uint32_t, std::string> & addr) {
    sockaddr_in server = get_addr(addr);
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        throw std::runtime_error(LOG("socket failed"));
    }

    if (::connect(client_sock, (sockaddr*)&server, sizeof(server)) < 0) {
        throw std::runtime_error("connect failed");
    }
};

struct sockaddr_in TCPTransceiver::get_addr(const std::pair<uint32_t, std::string> & addr) {
    return get_addr(addr.first, addr.second);    
};
struct sockaddr_in TCPTransceiver::get_addr(uint32_t port, const std::string & ip_addr) {
    sockaddr_in w{};
    w.sin_family = AF_INET;
    w.sin_port = htons(port);
    w.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    return w;
};

void TCPTransceiver::create_socket() {
    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock < 0) {
        throw std::runtime_error("Could't open socket");
    }
};


void TCPTransceiver::close_socket() {
    close_connection();
    close(server_sock);
};

void TCPTransceiver::close_connection() {
    if (client_sock >= 0)  {
        close(client_sock);
        client_sock  = -1;
    }
};
std::pair<uint32_t, std::string> TCPTransceiver::get_netaddr() const {
    return server;
};


void TCPTransceiver::send_msg(const std::string & msg) {
    if (client_sock < 0) {
        throw std::runtime_error(LOG("send_msg: TCP connection isn't estableshed"));
    }
    ssize_t sent = send(client_sock, msg.c_str(), msg.size(), 0);
    if (sent < 0) {
        throw std::runtime_error(LOG("send_msg: send failed"));
    }
}

std::string TCPTransceiver::receive_msg() {
    if (client_sock < 0) {
        throw std::runtime_error(LOG("receiv_msg: TCP connection isn't estableshed"));
    }
    char buf[buff_size];
    ssize_t size = recv(client_sock, buf, sizeof(buf), 0);
    if (size < 0) {
        throw std::runtime_error(LOG("receive_msg: recv"));
    }
    if (size == 0) {
        close_connection();
        throw std::runtime_error(LOG("receive_msg: peer disconnected"));
    }
    return std::string(buf, size);
};
