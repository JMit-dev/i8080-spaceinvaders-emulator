#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>

int disassemble8080(uint8_t *codeBuffer, uint16_t pc);
void disassembleROM(const char *filename);

#endif
