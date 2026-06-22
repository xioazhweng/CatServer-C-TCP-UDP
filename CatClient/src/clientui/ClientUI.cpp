#include "ClientUI.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

void ClientUI::run() {
    auto screen = ScreenInteractive::Fullscreen();
    auto input_message = Input(&params.message, "Message");

    int protocol_idx = 1; // 0=TCP, 1=UDP
    std::vector<std::string> protocol_labels = {"TCP", "UDP"};
    auto protocol_selector = Radiobox(&protocol_labels, &protocol_idx);

    auto send_button = Button("SEND", [&] {
        params.protocol = (protocol_idx == 0) ? "TCP" : "UDP";
        is_send.store(true);
    });

    auto form = Container::Vertical({input_message, protocol_selector, send_button});

    auto log_renderer = Renderer([&] {
        Elements lines;
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            for (auto & l : logs)
                lines.push_back(text(l));
        }
        return vbox(lines) | vscroll_indicator | frame | flex;
    });

    auto renderer = Renderer(form, [&] {
        return hbox({
            vbox({
                text("Cat Network Tool") | bold,
                separator(),
                text("Local  IP:   " + params.local_ip),
                text("Local  port: " + params.local_port),
                separator(),
                text("Remote: " + params.remote_ip + ":" + params.remote_port),
                separator(),
                hbox({text("Message: "), input_message->Render()}),
                separator(),
                text("Protocol:"),
                protocol_selector->Render(),
                separator(),
                send_button->Render(),
            }) | border | size(WIDTH, EQUAL, 44),
            vbox({
                text("LOGS") | bold,
                separator(),
                log_renderer->Render(),
            }) | border | flex,
        });
    });

    screen.Loop(renderer);
}

void ClientUI::add_log(const std::string & msg) {
    std::lock_guard<std::mutex> lock(log_mutex);
    logs.push_back(msg);
    while (logs.size() > MAX_LOG_SIZE)
        logs.pop_front();
}
