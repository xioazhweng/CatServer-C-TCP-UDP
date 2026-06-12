#include "UDPTransceiver.h"
#include <stdexcept>
#include <cstring>


void UDPTransceiver::create_socket() {
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd < 0) {
        throw std::runtime_error("Could't open socket");
    }
};

void UDPTransceiver::close_socket() {
    close(sd);
};

UDPTransceiver::UDPTransceiver(uint32_t server_port_, const std::string & server_ip_addr_): port(server_port_), ip(server_ip_addr_) {
    create_socket();
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<uint16_t>(port));
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) != 1) {
        throw std::runtime_error("Invalid IP address: " + ip);
    }
    
    if (bind(sd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        throw std::runtime_error("Bind failed: " + ip + ":" + std::to_string(port) + 
                               " - " + strerror(errno));
    }
}

ssize_t UDPTransceiver::send(const sockaddr_in & addr, const char * buffer, size_t len) {
    return sendto(sd, buffer, len, 0, (struct sockaddr*) &addr, sizeof(addr));
};
ssize_t UDPTransceiver::receive(sockaddr_in & addr, char * buffer, size_t len) {
    socklen_t addr_len = sizeof(addr);
    return recvfrom(sd, buffer, len, 0, 
                    reinterpret_cast<struct sockaddr*>(&addr), 
                    &addr_len);
};
UDPTransceiver::~UDPTransceiver() {
    close_socket();
};