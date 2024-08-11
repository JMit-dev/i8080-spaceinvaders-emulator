#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

int disassemble8080(unsigned char *codeBuffer, int pc);
void disassembleROM(const char *filename);

#endif
