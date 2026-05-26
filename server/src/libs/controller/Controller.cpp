#include "Controller.h"
#include <iostream>



Controller::Controller(size_t udpport, size_t tcpport, const std::string & ip): params_(udpport, tcpport, ip) {
    udpt = std::make_unique<UDPTransceiver>(udpport, ip);
    feedq = std::make_unique<FeedQueue>();
};

void Controller::run() {
     //UDPTransceiver feeder(12321, "192.168.1.109");
    //feeder.send_msg({9000, "144.31.132.123"}, "suka");
  //  feeder.feed_answer("suka");
   
    FeedQueue que;
    try {
        que.push("@Alex - Milk~");
        auto elem = que.front();
        std::cerr << elem.first << elem.second << std::endl;
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}