#include "SpaceCat.h"

bool SpaceCat::is_preffered_food(FoodType ft) {
    auto it = preffered_food.find(ft);
    if (it == preffered_food.end()) {
        return false;
    }
    return true;
};

void SpaceCat::feed(FoodType ft) {
    if (is_preffered_food(ft)) {
        pleased = true;
    } else {
        pleased = false;
    }
}