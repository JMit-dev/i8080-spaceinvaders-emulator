#include "io.h"

uint8_t readPort(uint8_t port) {
    switch (port) {
        case 1:
            return 0x00;
        case 2:
            return 0x00;
        case 3:
            return 0x00;
        default:
            return 0x00;
    }
}

void writePort(uint8_t port, uint8_t value) {
    switch (port) {
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            break;
    }
}
