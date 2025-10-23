#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "input.h"

// Space Invaders I/O port handling
// Manages hardware shift register and maps input ports

typedef struct IOState {
    uint16_t shift_register;
    uint8_t shift_offset;
    Input* input;
} IOState;

IOState* io_create(Input* input);
void io_destroy(IOState* io);

uint8_t io_read_port(IOState* io, uint8_t port);
void io_write_port(IOState* io, uint8_t port, uint8_t value);

#endif
