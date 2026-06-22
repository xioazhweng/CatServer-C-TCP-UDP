#pragma once
#include "Food.h"
#include <deque>
#include <map>
#include <string>

enum class FeedPushResult { FRAGMENT, COMPLETE, ERROR };

struct FeedMessage {
    std::string user;
    FoodType    food;
};

class FeedQueue {
public:
    FeedQueue() : last_fragment_(-1) {}

    // out_fragment_id заполняется только при FRAGMENT
    FeedPushResult push(const std::string & msg, int & out_fragment_id);

    FeedMessage & front() { return data_.front(); }
    void pop()            { data_.pop_front(); }
    bool empty() const    { return data_.empty(); }

private:
    std::deque<FeedMessage>    data_;
    std::map<int, std::string> fragments_;
    int                        last_fragment_;

    std::string rebuild();
};
