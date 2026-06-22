#pragma once
#include "food/Food.h"
#include <unordered_set>
#include <string>

class CatService {
public:
    
    static constexpr const char * NAME = "Fluffy";

    static const std::unordered_set<FoodType> & liked_food() {
        static const std::unordered_set<FoodType> foods = {
            FoodType::FISH,
            FoodType::MILK,
        };
        return foods;
    }
    

    bool is_liked(FoodType ft) const { return liked_food().count(ft) > 0; }
    const char * name()        const { return NAME; }
};
