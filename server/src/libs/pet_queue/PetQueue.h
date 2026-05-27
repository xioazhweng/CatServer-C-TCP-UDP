#pragma once
#include <deque>
#include <vector>
#include <string>


class PetQueue {
    private:
        std::deque<std::string> data;
        std::vector<std::string> fragments;
        std::string rebuild();
    public:
        PetQueue() {};
        void push(const std::string & msg);
        void pop() {
            data.pop_front();
        };
        std::string & front() {
            return data.front();
        }
        bool empty() const {
            return data.empty();
        }
       
};
