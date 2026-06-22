#pragma once
#include <string>
#include <mutex>
#include <unordered_set>
#include <fstream>
#include <ctime>

#include "Food.h"
#include "CatService.h"
#include "Logger.h"

class Controller {
public:
    Controller(int udp_port, int tcp_port, const std::string & ip);
    void run();

private:
    int         udp_port_;
    int         tcp_port_;
    std::string ip_;

    CatService  cat_;
    Logger      logger_;

    std::mutex                      fed_mutex_;
    std::unordered_set<std::string> fed_users_;

    std::mutex  stat_mutex_;

    void run_udp();
    void run_tcp();
    void write_stat(const std::string & event,
                    const std::string & user,
                    const std::string & detail);
};
