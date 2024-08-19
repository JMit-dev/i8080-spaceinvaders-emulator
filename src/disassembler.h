#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>

int disassemble8080(uint8_t*, uint16_t);
void disassembleROM(const char*);

#endif
