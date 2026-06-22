#pragma once
#include <string>

enum class FoodType {
    NONE, FISH, MILK, MEAT, BREAD, CARROT, BEER, MARTINI
};

FoodType get_type(const std::string & str);
std::string get_name(FoodType ft);
