#include "Controller.h"
#include <iostream>
#include <thread>

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
            auto dt = udpt->receive_msg();
            std::mutex udp_mutex;
            {
                std::lock_guard<std::mutex> m(udp_mutex);
                feedq[dt.addr_].push(dt.msg_);
                if (feedq[dt.addr_].empty()) {
                    continue;
                }
                for (auto & [key, value] : feedq) {
                    if (value.empty()) {
                        continue;
                    }
                    std::pair<FoodType, std::string> & info = value.front();
                    if (!cat->is_pleased(info.first)) {
                        udpt->send_msg(key, "Cat dissapointed by " + info.second);
                    } else {
                        udpt->send_msg(key, "Cat amused by " + info.second);
                    }
                    value.pop();
                }
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