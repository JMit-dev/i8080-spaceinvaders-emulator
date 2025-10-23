#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>
#include <stdio.h>

int disassemble8080(uint8_t* codeBuffer, uint16_t pc, FILE* output);
void disassembleROM(const char* filename);
void disassemble_space_invaders(FILE* output);

#endif
