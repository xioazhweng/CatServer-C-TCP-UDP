#pragma once

#include <string>
#include "dto/dto.h"
class JSONStateService {
    private:
        std::string config_path;
        CatInfo cat_info;
    public:
        JSONStateService(const std::string & str): config_path(str) {};
        CatInfo load_catinfo();
};