#include "Controller.h"
#include <thread>
#include <mutex>
#include <stdexcept>
#include <cstring>

void NetworkController::listen_udp() {
    while (listening) {
        ssize_t size = udp_client->receive(server_addr, buffer, buffer_size);
        if (size <= 0) {
            std::lock_guard<std::mutex> mt(log_mutex);
            logs.push_front("Couldn't receive message: " + std::string(strerror(errno)));
        } else
        { 
            std::lock_guard<std::mutex> mt(que_mutex);
            que.push({MSGType::UDP, std::string(buffer)});
        }
    }
};

void NetworkController::listen_tcp() {
    while (listening) {
        ssize_t size = tcp_stream->receive(buffer, buffer_size);
        if (size <= 0) {
            std::lock_guard<std::mutex> mt(log_mutex);
            logs.push_front("Couldn't receive message: " + std::string(strerror(errno)));
        } else
        { 
            std::lock_guard<std::mutex> mt(que_mutex);
            que.push({MSGType::UDP, std::string(buffer)});
        }
    };
};

#include <iostream>
void NetworkController::send(const std::pair<MSGType, std::string> & unit) {
    ssize_t size;
    if (unit.first == MSGType::UDP) {
        std::cout << "IP: " << inet_ntoa(server_addr.sin_addr) << "\n"
          << "Port: " << ntohs(server_addr.sin_port) << "\n"
          << "Family: " << (server_addr.sin_family == AF_INET ? "IPv4" : "Other") << "\n"
          << "Address raw: 0x" << std::hex << ntohl(server_addr.sin_addr.s_addr) << std::dec << std::endl;
        size = udp_client->send(server_addr, unit.second.c_str(), unit.second.size());
    } else if (unit.first == MSGType::TCP) {       
        tcp_stream->send(unit.second.c_str(), unit.second.size());
    }
    if (size <= 0) {
        std::lock_guard<std::mutex> mt(log_mutex);
        logs.push_front("Couldn't send message: " + std::string(strerror(errno)));
    };
}

void NetworkController::listen() {
    listening = true;
    udp_thread = std::thread(&NetworkController::listen_udp, this);
    if (tcp_stream != nullptr)
        tcp_thread = std::thread(&NetworkController::listen_tcp, this);

};

bool NetworkController::back_queue(std::pair<MSGType, std::string> & out) {
    std::lock_guard<std::mutex> lock(que_mutex);

    if (que.empty())
        return false;

    out = que.back();
    que.pop();
    return true;
}

bool  NetworkController::back_logs(std::string & out) {
    std::lock_guard<std::mutex> lock(log_mutex);
    if (logs.empty())
        return false;
    out = logs.back();
    logs.pop_back();
    return true;
};