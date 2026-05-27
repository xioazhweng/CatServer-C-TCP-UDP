#pragma once
#include <string>
#include <unordered_map>

class Repository {
    private:
        std::unordered_map<std::string, int> users;
    public:
        void add(const std::string &);
        void substruct(const std::string &);

};