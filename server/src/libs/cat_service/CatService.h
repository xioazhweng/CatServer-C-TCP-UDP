#pragma once

#include <cat/Cat.h>

class CatService {
    private:
        Cat & cat;
    public:
        //bool is_pleased() {return cat.is_pleased();};
        void feed(FoodType ft) {cat.feed(ft);};

};