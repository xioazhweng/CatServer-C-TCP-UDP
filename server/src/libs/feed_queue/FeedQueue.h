#pragma once
#include "food/Food.h"
#include <deque>
#include <vector>
#include <map>
#include <string>


class FeedQueue {
    private:
        std::deque<std::pair<FoodType, std::string>> data;
        std::map<int, std::string> fragments;
        int lastFragment;
        std::string rebuild();
    public:
        FeedQueue(): lastFragment(-1) {};
        void push(const std::string & msg);
        void pop() {
            data.pop_front();
        };
        std::pair<FoodType, std::string> & front() {
            return data.front();
        }
        bool empty() const {
            return data.empty();
        }
        int get_last_fragment() const {return lastFragment;};
};
