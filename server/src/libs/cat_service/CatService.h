#pragma once

#include <cat/Cat.h>

class CatService {
    private:
        Cat & cat;

    public:
        bool is_pleased();
};