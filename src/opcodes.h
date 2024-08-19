#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

#include "i8080.h"

int parity(uint8_t);
void logicFlags(State8080*);
void arithFlags(State8080*, uint16_t);
void bcdArithFlags(State8080*, uint16_t);

void ADC(State8080*, uint8_t);
void ADD(State8080*, uint8_t);
void ANA(State8080*, uint8_t);
void CALL(State8080*, uint8_t*);
void CMA(State8080*);
void CMC(State8080*);
void CMP(State8080*, uint8_t);
void DAA(State8080*);
void DAD(State8080*, uint32_t);
void DCR(State8080*, uint8_t*);
void DCX(uint8_t*, uint8_t*);
void INR(State8080*, uint8_t*);
void INX(uint8_t*, uint8_t*);
void JMP(State8080*, uint8_t*);
void LDA(State8080*, uint8_t*);
void LDAX(State8080*, uint8_t*, uint8_t*);
void LHLD(State8080*, uint8_t*);
void LXI(uint8_t*, uint8_t*, uint8_t*);
void ORA(State8080*, uint8_t);
void POP(State8080*, uint8_t*, uint8_t*);
void POP_PSW(State8080*);
void PUSH(State8080*, uint8_t, uint8_t);
void PUSH_PSW(State8080*);
void RAL(State8080*);
void RAR(State8080*);
void RET(State8080*);
void RLC(State8080*);
void RRC(State8080*);
void SHLD(State8080*, uint8_t*);
void STA(State8080*, uint8_t*);
void SBB(State8080*, uint8_t);
void SUB(State8080*, uint8_t);
void XCHG(State8080*);
void XRA(State8080*, uint8_t);
void XTHL(State8080*);

#endif
