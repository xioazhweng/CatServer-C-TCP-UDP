#include <iostream>
#include <thread>
#include "ClientUI.h"
#include "Controller.h"
#include "SRVResolver.h"


static const std::string DOMAIN     = "cat.local";
static const std::string CLIENT_IP  = "0.0.0.0";
static const uint32_t    CLIENT_UDP = 55000;


int main() {
    try {
        // Получаем хост и порты из DNS SRV-записей
        SRVRecord udp_srv = query_srv("_feed-the-cat._udp." + DOMAIN);
        SRVRecord tcp_srv = query_srv("_pet-the-cat._tcp."  + DOMAIN);

        std::string server_host = udp_srv.host;
        uint32_t    server_udp  = udp_srv.port;
        uint32_t    server_tcp  = tcp_srv.port;

        ClientParams params(
            CLIENT_IP,  std::to_string(CLIENT_UDP),
            server_host, std::to_string(server_udp)
        );
        ClientUI ui(params);

        NetworkController ctr(
            CLIENT_IP,   CLIENT_UDP,
            server_host, server_udp, server_tcp
        );

        std::thread ui_thread(&ClientUI::run, &ui);

        ui.add_log("WELCOME TO CATCLIENT!");
        ui.add_log("SRV  -> " + server_host);
        ui.add_log("UDP  -> " + std::to_string(server_udp));
        ui.add_log("TCP  -> " + std::to_string(server_tcp));

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
