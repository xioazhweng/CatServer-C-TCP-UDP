#pragma once
#include <memory>
#include "../libs/udptransceiver/UDPTransceiver.h"
#include "../libs/feed_queue/FeedQueue.h"


struct Params {
    size_t udp_port;
    size_t tcp_port;
    std::string ipv4;
    Params(const std::string & ip): udp_port(32132), tcp_port(45341) {};
    Params(size_t udpport, size_t tcpport, const std::string & ip): udp_port(udpport),
    tcp_port(tcpport), ipv4(ip) {};
};

class Controller {
    private:
        Params params_;
        std::unique_ptr<UDPTransceiver> udpt;
        std::unique_ptr<FeedQueue> feedq;
        void init();
    public:
        //Controller(Params & params);
        Controller(size_t udp_port, size_t tcp_port, const std::string & ipv4);
        void run();
};