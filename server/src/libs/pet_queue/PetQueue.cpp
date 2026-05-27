#include "PetQueue.h"
#include "food/Food.h"

#include <stdexcept>
#include <regex>
#include <iostream>

std::string PetQueue::rebuild() {
    std::string res = "";
    for (auto & part : fragments)
        res += part;
    return res;
}

void PetQueue::push(const std::string & str) {
    static std::regex pattern(
        R"(^@([A-Za-z0-9]+)~$)"
    );
    static std::regex final(R"(^(.+)~$)");
    std::smatch match;
    std::string msg;
    //целая строка
    if (std::regex_match(msg, match, pattern)) {
        data.push_back(match[1]);
        fragments.clear(); 
        return;
    }

    if (std::regex_match(str, match, final)) {
        msg = rebuild();    
        msg += str;         
    } else { //фрагмент
        fragments.push_back(str);   
        return;
    }
    
    if (std::regex_match(msg, match, pattern)) {
        data.push_back(match[1]);
        fragments.clear(); 
        return;
    }


    throw std::runtime_error("Message wrong format: final");
};