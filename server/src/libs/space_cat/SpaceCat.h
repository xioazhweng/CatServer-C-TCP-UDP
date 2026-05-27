#pragma once
#include "cat/Cat.h"
#include "food/Food.h"
#include <unordered_set>

class SpaceCat: Cat {
    private:
        std::unordered_set<FoodType> preffered_food; 
        bool pleased; 
        bool is_preffered_food(FoodType ft);
    public:
        SpaceCat(): pleased(false) {};
        void pet() override {;};
        void feed(FoodType ft) override;
        bool is_pleased(FoodType ft) override {return pleased;};
        void set_pleased(bool fl) override {pleased = fl;};
};