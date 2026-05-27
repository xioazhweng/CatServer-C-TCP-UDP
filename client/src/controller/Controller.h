#pragma once
#include <string>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include "../libs/udptransceiver/UDPTransceiver.h"

enum MSGType {
    TCP,
    UDP,
    NONE
};

struct CDataGram {
    std::string ip;
    std::string msg;
    unsigned port;
    MSGType type;
    CDataGram(unsigned p, const std::string & ip_, 
            const std::string & m, MSGType t):
            ip(ip_), port(p), msg(m), type(t) {};
};

class Controller {
    private:
        std::mutex que_mutex;
        std::atomic<bool> listening = {false};
        std::thread udp_thread;
        std::string ipv4;
        std::queue<CDataGram> que;
        std::unique_ptr<UDPTransceiver> udpt;
        unsigned port;
        void listen_udp();
        void listen_tcp();
    public:
        Controller(std::string ip): ipv4(ip), port(0) {
            udpt = std::make_unique<UDPTransceiver>(0, ip);
        };
        Controller(unsigned p, std::string ip): ipv4(ip), port(p) {
            udpt = std::make_unique<UDPTransceiver>(p, ip);
        };
        void send(unsigned p, const std::string & ip, MSGType type, const std::string & msg);
        void send(const CDataGram & dt);
        bool is_msg_queue_empty() {
             std::lock_guard<std::mutex> lock(que_mutex);
            return que.empty();};
        void pop_queue();
        bool back_queue(CDataGram &out);
        //std::queue<CDataGram> & get_queue() {return que;};
        void listen();
        void stop_listening() {listening = false;};
        
};