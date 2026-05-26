#include "Controller.h"
#include <iostream>



Controller::Controller(size_t udpport, size_t tcpport, const std::string & ip): params_(udpport, tcpport, ip) {
    udpt = std::make_unique<UDPTransceiver>(udpport, ip);
    feedq = std::make_unique<FeedQueue>();
    logger = std::make_unique<Logger>("../../log.txt");
};

void Controller::run() {
     //UDPTransceiver feeder(12321, "192.168.1.109");
    //feeder.send_msg({9000, "144.31.132.123"}, "suka");
  //  feeder.feed_answer("suka");
   
    try {
        auto dt = udpt->receive_msg();
        logger->log(Level::INFO, dt.msg_);
        feedq->push(dt.msg_);
        auto elem =  feedq->front();
        udpt->send_msg(dt.addr_, "suka");
        // << elem.first << elem.second << std::endl;
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}