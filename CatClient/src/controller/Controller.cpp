#include "Controller.h"
#include <stdexcept>
#include <cstring>

void NetworkController::listen_udp() {
    sockaddr_in from{};
    while (listening_) {
        ssize_t n = udp_client_->receive(from, buffer_, buffer_size);
        if (n <= 0) {
            std::lock_guard<std::mutex> lk(log_mutex_);
            logs_.push_front("UDP recv error: " + std::string(strerror(errno)));
        } else {
            std::lock_guard<std::mutex> lk(que_mutex_);
            que_.push({MSGType::UDP, std::string(buffer_, n)});
        }
    }
}

void NetworkController::listen_tcp() {
    while (listening_) {
        if (!tcp_stream_) break;
        ssize_t n = tcp_stream_->receive(buffer_, buffer_size);
        if (n <= 0) {
            std::lock_guard<std::mutex> lk(log_mutex_);
            logs_.push_front("TCP connection closed");
            break;
        } else {
            std::lock_guard<std::mutex> lk(que_mutex_);
            que_.push({MSGType::TCP, std::string(buffer_, n)});
        }
    }
}

void NetworkController::send(const std::pair<MSGType, std::string> & unit) {
    ssize_t n = 0;
    if (unit.first == MSGType::UDP) {
        n = udp_client_->send(udp_server_addr_, unit.second.c_str(), unit.second.size());
    } else if (unit.first == MSGType::TCP) {
        if (!tcp_stream_) {
            std::lock_guard<std::mutex> lk(log_mutex_);
            logs_.push_front("TCP: no connection");
            return;
        }
        n = tcp_stream_->send(unit.second.c_str(), unit.second.size());
    }
    if (n <= 0) {
        std::lock_guard<std::mutex> lk(log_mutex_);
        logs_.push_front("Send error: " + std::string(strerror(errno)));
    }
}

void NetworkController::listen() {
    listening_ = true;
    udp_thread_ = std::thread(&NetworkController::listen_udp, this);
    if (tcp_stream_)
        tcp_thread_ = std::thread(&NetworkController::listen_tcp, this);
}

bool NetworkController::back_queue(std::pair<MSGType, std::string> & out) {
    std::lock_guard<std::mutex> lk(que_mutex_);
    if (que_.empty()) return false;
    out = que_.front();
    que_.pop();
    return true;
}

bool NetworkController::back_logs(std::string & out) {
    std::lock_guard<std::mutex> lk(log_mutex_);
    if (logs_.empty()) return false;
    out = logs_.back();
    logs_.pop_back();
    return true;
}
