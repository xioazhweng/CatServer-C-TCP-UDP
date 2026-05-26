
#include <iostream>
#include "libs/controller/Controller.h"

int main(void) {
    Controller ctr(43311, 9000, "192.168.1.109");
    try {
      ctr.run();
    } catch (std::exception & e) {
      std::cerr << e.what();
    }
    return 0;
}