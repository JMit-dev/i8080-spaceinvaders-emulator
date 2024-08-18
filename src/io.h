#ifndef IO_H
#define IO_H

#include <stdint.h>

#include "i8080.h"

static uint8_t inputPort1 = 0x00;
static uint8_t inputPort2 = 0x00;
static uint8_t shiftRegister = 0x00;
static uint8_t shiftOffset = 0;

void initializeIO(void);
void shutdownIO(void);
uint8_t readPort(uint8_t port);
void writePort(uint8_t port, uint8_t value);
void handleInput(void);

#endif
