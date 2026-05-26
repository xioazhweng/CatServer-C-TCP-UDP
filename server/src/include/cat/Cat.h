#pragma once

class Cat {
    public:
        virtual void pet() = 0;
        virtual void feed() = 0;
        virtual bool is_pleased() = 0;
};