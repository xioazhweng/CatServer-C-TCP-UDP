#include <iostream>
#include "controller/Controller.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int main(int argc, char * argv[]) {
    if(argc < 3) {
        std::cerr << "Usage: ./catclient ip port" << std::endl;
        return 1;
    }
    
    auto screen = ScreenInteractive::FitComponent();
    std::string local_ip(argv[1]);
    std::string local_port = argv[2];
    MSGType type = MSGType::UDP;
    
    Controller ctr(std::stoi(local_port), local_ip);
    std::string remote_ip = "127.0.0.1";
    std::string remote_port = "6000";

    std::string message;

    int protocol = 0; // 0 = TCP, 1 = UDP

    
    auto input_local_ip    = Input(&local_ip, "Local IP");
    auto input_local_port  = Input(&local_port, "Local Port");

    auto input_remote_ip   = Input(&remote_ip, "Remote IP");
    auto input_remote_port  = Input(&remote_port, "Remote Port");

    auto input_message     = Input(&message, "Message");


    std::vector<std::string> protocol_labels = {"TCP", "UDP"};
    auto protocol_selector = Radiobox(&protocol_labels, &protocol);
    type = protocol == 0 ? MSGType::UDP : MSGType::TCP;
   
    auto send_button = Button("SEND", [&] {

        std::cout << "=== SEND ===\n";
        std::cout << "Local:  " << local_ip << ":" << local_port << "\n";
        std::cout << "Remote: " << remote_ip << ":" << remote_port << "\n";
        std::cout << "Proto:  " << (protocol == 0 ? "TCP" : "UDP") << "\n";
        std::cout << "Msg:    " << message << "\n\n";


        // TODO: call your socket layer here
    });

    auto form = Container::Vertical({

        input_local_ip,
        input_local_port,

        input_remote_ip,
        input_remote_port,

        input_message,

        protocol_selector,
        send_button,
    });

    auto renderer = Renderer(form, [&] {
        return vbox({

            text("Cat Network Tool") | bold,
            separator(),

            hbox({text("Local IP: "), input_local_ip->Render()}),
            hbox({text("Local Port: "), input_local_port->Render()}),

            separator(),

            hbox({text("Remote IP: "), input_remote_ip->Render()}),
            hbox({text("Remote Port: "), input_remote_port->Render()}),

            separator(),

            hbox({text("Message: "), input_message->Render()}),

            separator(),

            text("Protocol:"),
            protocol_selector->Render(),

            separator(),

            send_button->Render(),

        }) | border;
    });

    screen.Loop(renderer);
}