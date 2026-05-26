
#include <iostream>
#include "libs/controller/Controller.h"

int main(void) {
    Controller ctr(12321, 9000, "144.31.132.123");
    try {
      ctr.run();
    } catch (std::exception & e) {
      std::cerr << e.what();
    }
    return 0;
}