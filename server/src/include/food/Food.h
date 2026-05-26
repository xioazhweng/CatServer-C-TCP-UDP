#pragma once
#include <string>

enum FoodType {
    NONE,
    FISH,
    MILK,
    MEAT,
    BREAD,
    CARROT,
    BEER,
    MARTINI
};

FoodType get_type(std::string str);
std::string get_name(FoodType ft);