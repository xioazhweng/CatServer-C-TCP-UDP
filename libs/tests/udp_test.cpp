
#include "../udptransceiver/libs/UDPTransceiver.h"
#include<iostream>
#include <cstring>

#define SERVER_IP "192.168.1.100"
#define SERVER_PORT 54434
#define CLIENT_IP "192.168.1.109"
#define CLIENT_PORT 54434


int main(void) {
    try {
        UDPTransceiver udp_client(CLIENT_PORT, CLIENT_IP);
        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));          
        addr.sin_family = AF_INET;               
        addr.sin_port = htons(34343);            
        inet_pton(AF_INET, "192.168.1.100", &addr.sin_addr);  
        udp_client.send(addr, "Hello", 5);
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}