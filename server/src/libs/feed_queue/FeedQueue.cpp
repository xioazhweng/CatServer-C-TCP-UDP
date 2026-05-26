#include "FeedQueue.h"
#include "food/Food.h"

#include <stdexcept>
#include <regex>
#include <string>

std::string FeedQueue::rebuild() {
    std::string res = "";
    for (auto & [id, part] : fragments)
        res += part;
    return res;
}

void FeedQueue::push(const std::string & str) {
    static std::regex fragment(R"(^(.+)~(\d+)$)");
    static std::regex final(R"(^(.+)~$)");
    std::smatch match;
    int id = -1;
    // fragment
    if (std::regex_match(str, match, fragment)) {
        std::string part = match[1];
        id = std::stoi(match[2]);
        fragments[id] = part;   
        lastFragment = id;
        return;
    }
    std::string msg;

    //final part
    if (std::regex_match(str, match, final)) {
        msg = rebuild();    
        msg += str;           
    } else {
        throw std::runtime_error("Message wrong format: final");
    }
    

    // parse final message
    static std::regex pattern(
        R"(^@([A-Za-z0-9]+)\s*-\s*([A-Za-z]+(?:\s+[A-Za-z]+)*)~$)"
    );

    if (std::regex_match(msg, match, pattern)) {

        FoodType type = get_type(match[2]);

        if (type == FoodType::NONE) {
            throw std::runtime_error("Unregistered food");
        }

        data.push_back({type, match[1]});

        fragments.clear(); 

        lastFragment = -1;
        return;
    }

    throw std::runtime_error("Message wrong format");
 
};