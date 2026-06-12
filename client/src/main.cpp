#include <iostream>

#include "clientui/ClientUI.h"
#include "controller/Controller.h"
/*#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <deque>
*/
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
    std::thread ui_thread;
    std::mutex m;
    ClientParams params(config.client_ip, std::to_string(config.client_port), 
                        config.server_ip, std::to_string(config.server_port));
    
    try {
        ClientUI ui(params);
        ui_thread = std::thread(&ClientUI::run, &ui);
        NetworkController ctr(config.client_port, config.client_ip, config.server_port, config.server_ip);
        ctr.listen();
        if (ui.shouldSend()) {
            std::lock_guard<std::mutex> lock(m);
            MSGType type = params.protocol == "TCP" ? MSGType::TCP : MSGType::UDP; 
            ctr.send({type, params.message});
            ui.add_log("SEND (" + params.protocol + "): " + params.message);
            ui.resetSendFlag();
        }
        if (!ctr.is_msg_queue_empty()) {
            std::pair<MSGType, std::string> out;
            std::string type = out.first == MSGType::TCP ? "TCP" : "UDP";
            if (ctr.back_queue(out)) {
                ui.add_log("SEND (" + type + "): " + out.second);
            };

            
        }
        if (!ctr.is_logs_queue_empty()) {
            std::string out;
            if (ctr.back_logs(out)) {
                ui.add_log("ERROR: " + out);
            };
        }
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        ui_thread.join();
        return 1;
    }
    ui_thread.join();
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