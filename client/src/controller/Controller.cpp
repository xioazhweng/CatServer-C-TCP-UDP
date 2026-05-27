#include "Controller.h"
#include <thread>
#include <mutex>
#include <stdexcept>

void Controller::listen_udp() {
    while (listening) {
        auto dt = udpt->receive_msg();
        {
            std::lock_guard<std::mutex> mt(que_mutex);
            que.push(CDataGram(dt.addr_.first, dt.addr_.second, dt.msg_, MSGType::UDP));
        }
    }

};
void Controller::listen_tcp() {

};
void Controller::pop_queue() {
    std::lock_guard<std::mutex> lock(que_mutex);
    if (que.empty()) {
        throw std::runtime_error("empty queue");
    }
    que.pop();  
};

void Controller::send(unsigned p, const std::string & ip, 
                      MSGType type, const std::string & msg) {
    if (type == MSGType::UDP) {
        udpt->send_msg({p, ip},msg);
    }
};
void Controller::send(const CDataGram & dt) {
    send(dt.port, dt.ip, dt.type, dt.msg);
};
void Controller::listen() {
    listening = true;
    udp_thread = std::thread(&Controller::listen_udp, this);

};
bool Controller::back_queue(CDataGram &out) {
    std::lock_guard<std::mutex> lock(que_mutex);

    if (que.empty())
        return false;

    out = que.back();
    return true;
}