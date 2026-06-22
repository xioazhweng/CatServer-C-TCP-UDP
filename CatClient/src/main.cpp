#include <iostream>
#include <thread>
#include "ClientUI.h"
#include "Controller.h"

static const std::string SERVER_HOST    = "catserver.cat.local";
static const uint32_t    SERVER_UDP     = 32132;
static const uint32_t    SERVER_TCP     = 45341;

static const std::string CLIENT_IP      = "0.0.0.0";
static const uint32_t    CLIENT_UDP     = 55000;

int main() {
    try {
        ClientParams params(
            CLIENT_IP,  std::to_string(CLIENT_UDP),
            SERVER_HOST, std::to_string(SERVER_UDP)
        );
        ClientUI ui(params);

        NetworkController ctr(
            CLIENT_IP,   CLIENT_UDP,
            SERVER_HOST, SERVER_UDP, SERVER_TCP
        );

        std::thread ui_thread(&ClientUI::run, &ui);

        ui.add_log("WELCOME TO CATCLIENT!");
        ui.add_log("Server: " + SERVER_HOST);
        ui.add_log("UDP -> " + std::to_string(SERVER_UDP));
        ui.add_log("TCP -> " + std::to_string(SERVER_TCP));

        ctr.listen();

        while (true) {
            if (ui.shouldSend()) {
                const auto & p = ui.get_params();
                if (p.message.empty()) {
                    ui.add_log("ERROR: empty message");
                } else if (p.protocol == "UDP") {
                    ctr.send({MSGType::UDP, p.message});
                    ui.add_log("SEND UDP: " + p.message);
                } else if (p.protocol == "TCP") {
                    ctr.send({MSGType::TCP, p.message});
                    ui.add_log("SEND TCP: " + p.message);
                }
                std::string l;
                while (ctr.back_logs(l)) ui.add_log(l);
                ui.resetSendFlag();
            }

            std::pair<MSGType, std::string> unit;
            while (ctr.back_queue(unit)) {
                std::string prefix = (unit.first == MSGType::UDP) ? "RECV UDP: " : "RECV TCP: ";
                ui.add_log(prefix + unit.second);
            }
        }

        ui_thread.join();
    } catch (const std::exception & e) {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return 1;
    }
}
