#pragma once
#include <memory>
#include <unordered_map>
#include "../libs/udptransceiver/UDPTransceiver.h"
#include "../libs/feed_queue/FeedQueue.h"
#include "../libs/pet_queue/PetQueue.h"
#include "../libs/logger/Logger.h"
#include "../libs/repository/Repository.h"
#include "../libs/cat_service/CatService.h"
#include "../libs/space_cat/SpaceCat.h"

struct Params {
    size_t udp_port;
    size_t tcp_port;
    std::string ipv4;
    Params(const std::string & ip): udp_port(32132), tcp_port(45341) {};
    Params(size_t udpport, size_t tcpport, const std::string & ip): udp_port(udpport),
    tcp_port(tcpport), ipv4(ip) {};
};

struct Users {
    std::pair<int, std::string> addr;
    FeedQueue feedq;
    Users(): feedq() {};
};

struct PairHash {
    std::size_t operator()(const std::pair<int, std::string>& p) const noexcept {
        return std::hash<int>{}(p.first) ^
               (std::hash<std::string>{}(p.second) << 1);
    }
};

class Controller {
    private:
        Params params_;
        std::unique_ptr<Logger> logger;
        std::unique_ptr<PetQueue> petq;
        std::unique_ptr<Repository> repo;
        std::unique_ptr<UDPTransceiver> udpt;
        std::unordered_map<std::pair<unsigned, std::string>, FeedQueue, PairHash> feedq;
       // std::unordered_map<std::pair<int, std::string>, PetQueue> petq;
        std::unique_ptr<SpaceCat> cat;
        void init();
    public:
        //Controller(Params & params);
        Controller(size_t udp_port, size_t tcp_port, const std::string & ipv4);
        void run();
        void run_udp();
        void run_tcp();
};