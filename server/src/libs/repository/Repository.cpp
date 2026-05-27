#include "Repository.h"

void Repository::add(const std::string & name) {
    users[name]++;
};

void Repository::substruct(const std::string & name) {
    auto it = users.find(name);
    if (it == users.end()) {
        return;
    }
    if (it->second == 1) {
        users.erase(it);
    } else {
        users[name]--;
    }
    
};