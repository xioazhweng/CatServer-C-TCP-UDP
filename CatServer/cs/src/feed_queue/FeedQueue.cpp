#include "FeedQueue.h"
#include <regex>

std::string FeedQueue::rebuild() {
    std::string res;
    for (auto & [id, part] : fragments_)
        res += part;
    return res;
}

FeedPushResult FeedQueue::push(const std::string & msg, int & out_fragment_id) {
    static std::regex re_frag (R"(^(.+)~(\d+)$)");
    static std::regex re_final(R"(^(.+)~$)");
    static std::regex re_msg  (R"(^@([A-Za-z0-9]+)\s*-\s*([A-Za-z]+(?:\s+[A-Za-z]+)*)~$)");

    std::smatch m;

    if (std::regex_match(msg, m, re_frag)) {
        int id = std::stoi(m[2]);
        fragments_[id] = m[1];
        last_fragment_ = id;
        out_fragment_id = id;
        return FeedPushResult::FRAGMENT;
    }

    std::string full;
    if (std::regex_match(msg, m, re_final)) {
        full = rebuild() + msg;
        fragments_.clear();
        last_fragment_ = -1;
    } else {
        return FeedPushResult::ERROR;
    }

    if (std::regex_match(full, m, re_msg)) {
        FoodType ft = get_type(m[2]);
        if (ft == FoodType::NONE) return FeedPushResult::ERROR;
        data_.push_back({m[1], ft});
        return FeedPushResult::COMPLETE;
    }

    return FeedPushResult::ERROR;
}
