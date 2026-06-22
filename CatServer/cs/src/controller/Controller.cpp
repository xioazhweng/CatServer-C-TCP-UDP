#include "Controller.h"
#include "feed_queue/FeedQueue.h"
#include "pet_queue/PetQueue.h"

#include <thread>
#include <unordered_map>

#include "../../libs/net/udptransceiver/libs/UDPTransceiver.h"
#include "../../libs/net/tcptransceiver/libs/tcpserver/TCPServer.h"

#define MAX_BUF 500
#define STAT_FILE "stats/stats.csv"

// ─── stats ──────────────────────────────────────────────────────────────────

void Controller::write_stat(const std::string & event,
                             const std::string & user,
                             const std::string & detail) {
    std::lock_guard<std::mutex> lk(stat_mutex_);

    // заголовок если файл новый
    std::ifstream check(STAT_FILE);
    bool is_new = !check.good();
    check.close();

    if (is_new) {
        std::ofstream f(STAT_FILE);
        f << "timestamp,event,user,detail\n";
    }

    // текущее время
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_r(&t, &tm);
    char ts[32];
    std::strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);

    std::ofstream f(STAT_FILE, std::ios::app);
    f << ts << "," << event << "," << user << "," << detail << "\n";
}


Controller::Controller(int udp_port, int tcp_port, const std::string & ip)
    : udp_port_(udp_port), tcp_port_(tcp_port), ip_(ip)
    , logger_("log.txt")
{
    system("mkdir -p stats");
    logger_.log(Level::INFO, std::string("Started. Cat: ") + cat_.name()
                             + " | UDP:" + std::to_string(udp_port_)
                             + " TCP:" + std::to_string(tcp_port_));
}


void Controller::run_udp() {
    UDPTransceiver udp(udp_port_, ip_);
    logger_.log(Level::INFO, "UDP ready");

    std::unordered_map<std::string, FeedQueue> queues;

    while (true) {
        try {
            sockaddr_in sender{};
            char buf[MAX_BUF];
            ssize_t n = udp.receive(sender, buf, MAX_BUF);
            if (n <= 0) continue;

            std::string msg(buf, n);
            std::string key = std::string(inet_ntoa(sender.sin_addr))
                              + ":" + std::to_string(ntohs(sender.sin_port));

            int frag_id = -1;
            FeedPushResult res = queues[key].push(msg, frag_id);

            std::string reply;
            switch (res) {
                case FeedPushResult::FRAGMENT:
                    reply = "The Cat is amused by #" + std::to_string(frag_id);
                    logger_.log(Level::INFO, key + " fragment #" + std::to_string(frag_id));
                    break;

                case FeedPushResult::COMPLETE: {
                    FeedMessage & fm = queues[key].front();
                    if (cat_.is_liked(fm.food)) {
                        reply = "Eaten by the Cat";
                        { std::lock_guard<std::mutex> lk(fed_mutex_);
                          fed_users_.insert(fm.user); }
                        write_stat("feed_accepted", fm.user, get_name(fm.food));
                    } else {
                        reply = "Ignored by the Cat";
                        write_stat("feed_rejected", fm.user, get_name(fm.food));
                    }
                    logger_.log(Level::INFO, key + " @" + fm.user
                                + " -> " + get_name(fm.food) + " -> " + reply);
                    queues[key].pop();
                    break;
                }

                case FeedPushResult::ERROR:
                    reply = "Bad request";
                    logger_.log(Level::WARN, key + " bad message: " + msg);
                    break;
            }

            udp.send(sender, reply.c_str(), reply.size());

        } catch (const std::exception & e) {
            logger_.log(Level::ERROR, std::string("UDP: ") + e.what());
        }
    }
}


void Controller::run_tcp() {
    TCPServer server(tcp_port_, ip_.c_str());
    if (server.start() != 0) {
        logger_.log(Level::ERROR, "TCP bind failed on port " + std::to_string(tcp_port_));
        return;
    }
    logger_.log(Level::INFO, "TCP ready");

    while (true) {
        auto stream = server.accept();
        if (!stream) continue;

        std::string peer = stream->getPeerIP() + ":"
                           + std::to_string(stream->getPeerPort());
        logger_.log(Level::INFO, "TCP connect: " + peer);

        std::thread([this, peer, s = std::move(stream)]() mutable {
            PetQueue pq;
            char buf[MAX_BUF];
            int  pets     = 0;
            const int MAX_PETS = 5;

            while (pets < MAX_PETS) {
                ssize_t n = s->receive(buf, MAX_BUF);
                if (n <= 0) break;

                if (!pq.push(std::string(buf, n)))
                    continue;

                std::string reply;
                while (!pq.empty() && pets < MAX_PETS) {
                    std::string user = pq.front(); pq.pop();

                    bool fed;
                    { std::lock_guard<std::mutex> lk(fed_mutex_);
                      fed = fed_users_.count(user) > 0; }

                    if (fed) {
                        reply += "Tolerated by the Cat";
                        write_stat("pet_tolerated", user, peer);
                        logger_.log(Level::INFO, peer + " @" + user + " tolerated");
                    } else {
                        reply += "Scratched by the Cat";
                        write_stat("pet_scratched", user, peer);
                        logger_.log(Level::INFO, peer + " @" + user + " scratched");
                    }
                    pets++;
                }

                if (!reply.empty())
                    s->send(reply.c_str(), reply.size());
            }

            logger_.log(Level::INFO, "TCP close: " + peer
                                     + " (pets=" + std::to_string(pets) + ")");
        }).detach();
    }
}


void Controller::run() {
    std::thread t1(&Controller::run_udp, this);
    std::thread t2(&Controller::run_tcp, this);
    t1.join();
    t2.join();
}
