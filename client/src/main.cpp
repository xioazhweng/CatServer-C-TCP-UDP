#include <iostream>
#include "udptransceiver/UDPTransceiver.h"

int main(void) {
    UDPTransceiver udpt(52144, "192.168.1.106");
    udpt.send_msg({43311, "192.168.1.109"}, "ustal");
}