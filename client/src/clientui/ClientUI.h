#pragma once 

#include <string>
#include <deque>
#include <atomic>
#include <functional>
#include <mutex>

struct ClientParams {
    std::string local_ip;
    std::string local_port;
    std::string remote_ip;
    std::string remote_port;
    std::string message;
    std::string protocol;
    ClientParams(std::string local_ip_, 
                std::string local_port_,
                std::string remote_ip_, 
                std::string remote_port_,
                std::string protocol_ = "tcp")
    : local_ip(std::move(local_ip_))
    , local_port(std::move(local_port_))
    , remote_ip(std::move(remote_ip_))
    , remote_port(std::move(remote_port_))
    , protocol(std::move(protocol_))
    {};
    ClientParams(const ClientParams& other)
        : local_ip(other.local_ip)
        , local_port(other.local_port)
        , remote_ip(other.remote_ip)
        , remote_port(other.remote_port)
        , message(other.message)
        , protocol(other.protocol)
    {}
};



class ClientUI {
    private:
        ClientParams params;
        std::atomic<bool> is_send{false};
        std::deque<std::string> logs;
        std::mutex log_mutex;
        std::string formatstr(unsigned port, const std::string & ip,
                 const std::string & msg, const std::string & type) {
                return type + ": " + ip + ":" + std::to_string(port) + " - " + msg;
        };
       

    public:
        ClientUI(const ClientParams & cp): params(cp) {};
        void run();
       ~ClientUI() = default;
        
        bool shouldSend() {
            return is_send.load();
        }
        
        void resetSendFlag() {
            is_send.store(false);
        }
        void add_log (std::string msg) {
            std::lock_guard<std::mutex> lock(log_mutex);
            logs.push_back(msg);
            if (logs.size() > 200)
                logs.pop_front();
        };
};