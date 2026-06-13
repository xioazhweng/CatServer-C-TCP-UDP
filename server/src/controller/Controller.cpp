#include "Controller.h"
#include <iostream>
#include <thread>
#include "../libs/udptransceiver/libs/UDPTransceiver.h"
Controller::Controller(size_t udpport, size_t tcpport, const std::string & ip): params_(udpport, tcpport, ip) {
    udpt = std::make_unique<UDPTransceiver>(udpport, ip);
   // feedq = std::make_unique<FeedQueue>();
    logger = std::make_unique<Logger>("../../log.txt");
  //  petq = std::make_unique<PetQueue>();
    repo = std::make_unique<Repository>();
    cat = std::make_unique<SpaceCat>();
};

void Controller::run_udp() {
    while (true) {
        try {
            sockaddr_in sender_addr;
            char buffer[MAX_BUF_SIZE];
            ssize_t size = udpt->receive(sender_addr, buffer, MAX_BUF_SIZE);
            if (size > 0) {
                std::lock_guard<std::mutex> m(feed_mutex);
                std::pair<unsigned, std::string> key = {ntohs(sender_addr.sin_port), inet_ntoa(sender_addr.sin_addr)};
                feedq[key].push(std::string(buffer, size));
                if (feedq[key].empty()) {
                    continue;
                }
                std::pair<FoodType, std::string> & info = feedq[key].front();
                std::string mesg;
                if (!cat->is_pleased(info.first)) {
                    mesg = "Cat dissapointed by " + info.second;
                } else {
                    mesg = "Cat amused by " + info.second;
                }
                size = udpt->send(sender_addr, mesg.c_str(), mesg.size());
                feedq[key].pop();
                
            }
            
        } catch (const std::exception & e) {
            logger->log(Level::ERROR, e.what());
        }

        
       
    }
    
    
};
void Controller::run_tcp() {

};

void Controller::run() {
     //UDPTransceiver feeder(12321, "192.168.1.109");
    //feeder.send_msg({9000, "144.31.132.123"}, "suka");
  //  feeder.feed_answer("suka");
    std::thread t1(&Controller::run_udp, this);
    std::thread t2(&Controller::run_tcp, this);
    t1.join();
    t2.join();
   

   
}