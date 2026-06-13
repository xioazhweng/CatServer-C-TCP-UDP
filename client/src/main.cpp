#include <iostream>

#include "clientui/ClientUI.h"
#include "controller/Controller.h"
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
//using namespace ftxui;
struct NetworkConfig {
    std::string server_ip;
    uint32_t server_port;
    std::string client_ip;
    uint32_t client_port;
    
    NetworkConfig() 
        : server_ip("127.0.0.1")
        , server_port(8080)
        , client_ip("0.0.0.0")
        , client_port(0) 
    {}
};
NetworkConfig loadConfig(const std::string& filename);


int main(void) {
    NetworkConfig config = loadConfig("../config/params.json");
    
    
    try {
        
        std::mutex m;
        ClientParams params(config.client_ip, std::to_string(config.client_port), 
                            config.server_ip, std::to_string(config.server_port));
        ClientUI ui(params);
       NetworkController ctr(config.client_port, config.client_ip, config.server_port, config.server_ip);
        std::thread ui_thread = std::thread(&ClientUI::run, &ui);
        ui.add_log("WELCOME TO CATCLIENT!");
                
        while(1) {
            if (ui.shouldSend()) {
                const auto & p = ui.get_params();
                if (p.message.empty()) {
                    ui.add_log("ERROR: Don't wanna send empty message");
                } else if (p.protocol == "UDP") {
                    ctr.send({MSGType::UDP, p.message});
                    ui.add_log("SEND: UDP " + std::to_string(p.message.size()) + p.message);

                } else if (p.protocol == "TCP") {
                    ctr.send({MSGType::TCP, p.message});
                    ui.add_log("SEND: TCP " + std::to_string(p.message.size()) + p.message);
                } else {
                    ui.add_log("ERROR: unknonw protocol");
                }
                std::string l;
                while (ctr.back_logs(l)) {
                    ui.add_log(l);
                }
                ui.resetSendFlag();
            }
        };
        ui_thread.join();
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
   
    return 0;
}

NetworkConfig loadConfig(const std::string& filename) {
    NetworkConfig config;
    
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + filename);
        }
        
        json j;
        file >> j;
        
        if (j.contains("server")) {
            config.server_ip = j["server"].value("ip", "127.0.0.1");
            config.server_port = j["server"].value("port", 8080);
        }
        
        if (j.contains("client")) {
            config.client_ip = j["client"].value("ip", "0.0.0.0");
            config.client_port = j["client"].value("port", 0);
        }
        
        std::cout << "Config loaded from " << filename << std::endl;
        
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        std::cerr << "Using default config" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        std::cerr << "Using default config" << std::endl;
    }
    
    return config;
}