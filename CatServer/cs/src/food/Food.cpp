#include "Food.h"

FoodType get_type(const std::string & str) {
    if (str == "Fish")    return FoodType::FISH;
    if (str == "Milk")    return FoodType::MILK;
    if (str == "Meat")    return FoodType::MEAT;
    if (str == "Bread")   return FoodType::BREAD;
    if (str == "Carrot")  return FoodType::CARROT;
    if (str == "Beer")    return FoodType::BEER;
    if (str == "Martini") return FoodType::MARTINI;
    return FoodType::NONE;
}

std::string get_name(FoodType ft) {
    switch (ft) {
        case FoodType::FISH:    return "Fish";
        case FoodType::MILK:    return "Milk";
        case FoodType::MEAT:    return "Meat";
        case FoodType::BREAD:   return "Bread";
        case FoodType::CARROT:  return "Carrot";
        case FoodType::BEER:    return "Beer";
        case FoodType::MARTINI: return "Martini";
        default:                return "None";
    }
}
