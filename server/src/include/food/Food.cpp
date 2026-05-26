#include "Food.h"

FoodType get_type(std::string str) {
    if (str == "Fish") return FISH;
    if (str == "Milk") return MILK;
    if (str == "Meat") return MEAT;
    if (str == "Bread") return BREAD;
    if (str == "Carrot") return CARROT;
    if (str == "Beer") return BEER;
    if (str == "Martini") return MARTINI;
    return NONE;
};

std::string get_name(FoodType ft) {
    switch (ft) {
        case FISH:    return "Fish";
        case MILK:    return "Milk";
        case MEAT:    return "Meat";
        case BREAD:   return "Bread";
        case CARROT:  return "Carrot";
        case BEER:    return "Beer";
        case MARTINI: return "Martini";
        default:      return "None";
    }
};