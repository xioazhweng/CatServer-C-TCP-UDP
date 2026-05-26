#include <stdexcept>
#include "UDPTransceiver.h"

UDPTransceiver::UDPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_): 
    server({server_port_, server_ip_addr_}) {
    create_socket();
    server_addr = get_addr(server);
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Bind failed");
    };
}
UDPTransceiver::~UDPTransceiver() {
    close_socket();
};

struct sockaddr_in UDPTransceiver::get_addr(const std::pair<uint32_t, std::string> & addr) {
    return get_addr(addr.first, addr.second);    
};
struct sockaddr_in UDPTransceiver::get_addr(uint32_t port, const std::string & ip_addr) {
    sockaddr_in w{};
    w.sin_family = AF_INET;
    w.sin_port = htons(port);
    w.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    return w;
};
struct sockaddr_in  UDPTransceiver::get_addr(uint32_t port, uint32_t ip_addr) {
    sockaddr_in w{};
    w.sin_family = AF_INET;
    w.sin_port = htons(port);
    w.sin_addr.s_addr = htonl(ip_addr);
    return w;
};

void UDPTransceiver::create_socket() {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        throw std::runtime_error("Could't open socket");
    }
};

void UDPTransceiver::close_socket() {
    close(sock);
};

std::pair<uint32_t, std::string> UDPTransceiver::get_netaddr() const {
    return server;
};

void UDPTransceiver::send_msg(const std::pair<uint32_t, std::string> & addr, const std::string & msg) {
    sockaddr_in servert = get_addr(addr);
    sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*) &servert, sizeof(servert));
}
void UDPTransceiver::send_msg(const DataGram & dg) {
    sockaddr_in servert = get_addr(dg.addr_);
    sendto(sock, dg.msg_.c_str(), dg.msg_.size(), 0, (struct sockaddr*) &servert, sizeof(servert));
}

DataGram UDPTransceiver::receive_msg() {
    char buf[buff_size];
    sockaddr_in client_addr{}; 
    socklen_t addr_len = sizeof(client_addr);
    ssize_t size = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, &addr_len);
    if (size < 0) {
        throw std::runtime_error("recvfrom failed");
    }
    return DataGram(client_addr, std::string(buf, size));
};
