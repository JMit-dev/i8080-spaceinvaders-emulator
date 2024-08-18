#ifndef IO_H
#define IO_H

#include <stdint.h>

uint8_t readPort(uint8_t port);
void writePort(uint8_t port, uint8_t value);

#endif
