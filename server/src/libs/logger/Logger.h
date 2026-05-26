#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
 enum class Level {
            INFO,
            WARN,
            ERROR
        };
        
class Logger {
    public:
       

        Logger(const std::string& filename = "")
            : use_file(!filename.empty()) {

            if (use_file) {
                file.open(filename, std::ios::app);
            }
        }

        ~Logger() {
            if (file.is_open()) {
                file.close();
            }
        }

        void log(Level level, const std::string& msg) {
            std::lock_guard<std::mutex> lock(mtx);

            std::string line = format(level, msg);

            if (use_file && file.is_open()) {
                file << line << std::endl;
            } else {
                std::cerr << line << std::endl;
            }
        }

    private:
        std::ofstream file;
        bool use_file = false;
        std::mutex mtx;

        std::string format(Level level, const std::string& msg) {
            return timestamp() + " [" + level_to_string(level) + "] " + msg;
        }

        std::string level_to_string(Level level) {
            switch (level) {
                case Level::INFO: return "INFO";
                case Level::WARN: return "WARN";
                case Level::ERROR: return "ERROR";
            }
            return "UNKNOWN";
        }

        std::string timestamp() {
        std::time_t now = std::time(nullptr);
        std::tm tm{};

    #if defined(_WIN32)
            localtime_s(&tm, &now);
    #else
            localtime_r(&now, &tm);
    #endif

            char buf[32];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
            return buf;
        }
};