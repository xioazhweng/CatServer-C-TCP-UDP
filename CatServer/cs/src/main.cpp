#include <iostream>
#include <regex>
#include "controller/Controller.h"

int main(int argc, char * argv[]) {
    static std::regex ipv4(
        "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){"
        "3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])"
    );

    if (argc < 4) {
        std::cerr << "Usage: ./catserver <udp_port> <tcp_port> <ipv4>" << std::endl;
        return 1;
    }
    if (!std::regex_match(argv[3], ipv4)) {
        std::cerr << "Invalid IP: " << argv[3] << std::endl;
        return 1;
    }

    try {
        Controller ctr(std::stoi(argv[1]), std::stoi(argv[2]), argv[3]);
        ctr.run();
    } catch (const std::exception & e) {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return 1;
    }
}
