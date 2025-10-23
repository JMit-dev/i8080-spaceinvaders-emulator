#include "io.h"
#include <stdlib.h>

IOState* io_create(Input* input) {
    IOState* io = malloc(sizeof(IOState));
    if (!io) return NULL;

    io->shift_register = 0x0000;
    io->shift_offset = 0;
    io->input = input;

    return io;
}

void io_destroy(IOState* io) {
    free(io);
}

uint8_t io_read_port(IOState* io, uint8_t port) {
    if (!io) return 0x00;

    switch (port) {
        case 1:
            return input_get_port1(io->input);
        case 2:
            return input_get_port2(io->input);
        case 3:
            // Hardware shift register read
            return (io->shift_register >> (8 - io->shift_offset)) & 0xFF;
        default:
            return 0x00;
    }
}

void io_write_port(IOState* io, uint8_t port, uint8_t value) {
    if (!io) return;

    switch (port) {
        case 2:
            // Set shift amount
            io->shift_offset = value & 0x07;
            break;
        case 4:
            // Shift register data write
            io->shift_register = (io->shift_register >> 8) | (value << 8);
            break;
        case 3:  // Sound
        case 5:  // Sound
        case 6:  // Debug port
            // Not implemented
            break;
        default:
            break;
    }
}