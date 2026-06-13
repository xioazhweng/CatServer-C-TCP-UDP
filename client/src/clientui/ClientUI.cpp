#include "ClientUI.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <deque>

using namespace ftxui;

void ClientUI::run() {
    auto screen = ScreenInteractive::Fullscreen();;
    auto input_message     = Input(&params.message, "Message");

    int protocol = 1;
    std::vector<std::string> protocol_labels = {"TCP", "UDP"};
    auto protocol_selector = Radiobox(&protocol_labels, &protocol);
    params.protocol = protocol == 0 ?  "TCP" : "UDP";
   
    auto send_button = Button("SEND", [&] {
        is_send.store(true);
        add_log("send clicked");
        std::cout << "=== SEND ===\n";
        std::cout << "Local:  " << params.local_ip << ":" << params.local_port << "\n";
        std::cout << "Remote: " << params.remote_ip << ":" << params.remote_port << "\n";
        std::cout << "Proto:  " << (protocol == 0 ? "TCP" : "UDP") << "\n";
        std::cout << "Msg:    " << params.message << "\n\n";
        
    });
    
    auto form = Container::Vertical({
        input_message,
        protocol_selector,
        send_button,
    });

    auto log_renderer = Renderer([&] {
        Elements lines;
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            for (auto &l : logs)
                lines.push_back(text(l));
            
        }
        return vbox(lines) | vscroll_indicator | frame | flex;
    });

    auto main = Container::Horizontal({
        form,
        log_renderer,
    });
    auto renderer = Renderer(form, [&] {
       
        return hbox({
            
            vbox({
                text("Cat Network Tool") | bold,
                separator(),
                text("Local IP: " + params.local_ip),
                text("Local port: " + params.local_port),
                separator(),
                text("Remote IP: " + params.remote_ip),
                text("Remote port: " + params.remote_port),
                separator(),
                hbox({text("Message: "), input_message->Render()}),
                separator(),
                text("Protocol:"),
                protocol_selector->Render(),
                separator(),
                send_button->Render(),

        }) |  border | size(WIDTH, EQUAL, 40),
        vbox({
                text("LOGS") | bold,
                separator(),
                log_renderer->Render(),
            }) | border | flex,
        });
    });
    
    screen.Loop(renderer);
}

void ClientUI::add_log (const std::string & msg) {
    std::lock_guard<std::mutex> lock(log_mutex);
    logs.push_back(msg);
    while (logs.size() > MAX_LOG_SIZE) {
        logs.pop_front();
    }
}
