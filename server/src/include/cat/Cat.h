#pragma once
#include "food/Food.h"

class Cat {
    public:
        virtual void pet() = 0;
        virtual void feed(FoodType ft) = 0;
        virtual bool is_pleased(FoodType ft) = 0;
        virtual void set_pleased(bool fl) = 0;
        ~Cat() = default;
};