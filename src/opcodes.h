#ifndef OPCODES_H
#define OPCODES_H

#include "i8080.h"

void ADC(State8080 *state, uint8_t addend);
void ADD(State8080 *state, uint8_t addend);
void ANA(State8080 *state, uint8_t var);
void CALL(State8080 *state, unsigned char *opcode);
void CMA(State8080 *state);
void CMC(State8080 *state);
void CMP(State8080 *state, uint8_t var);
void DAA(State8080 *state);
void DAD(State8080 *state, uint32_t regPair);
void DCR(State8080 *state, uint8_t *reg);
void DCX(uint8_t *msbReg, uint8_t *lsbReg);
void INR(State8080 *state, uint8_t *reg);
void INX(uint8_t *msbReg, uint8_t *lsbReg);
void JMP(State8080 *state, unsigned char *opcode);
void LDA(State8080 *state, unsigned char *opcode);
void LDAX(State8080 *state, uint8_t *msbReg, uint8_t *lsbReg);
void LHLD(State8080 *state, unsigned char *opcode);
void LXI(uint8_t *msbReg, uint8_t *lsbReg, unsigned char *opcode);
void ORA(State8080 *state, uint8_t var);
void POP(State8080 *state, uint8_t *msbReg, uint8_t *lsbReg);
void POP_PSW(State8080 *state);
void PUSH(State8080 *state, uint8_t msbReg, uint8_t lsbReg);
void PUSH_PSW(State8080 *state);
void RAL(State8080 *state);
void RAR(State8080 *state);
void RET(State8080 *state);
void RLC(State8080 *state);
void RRC(State8080 *state);
void SHLD(State8080 *state, unsigned char *opcode);
void STA(State8080 *state, unsigned char *opcode);
void SBB(State8080 *state, uint8_t subtrahend);
void SUB(State8080 *state, uint8_t subtrahend);
void XCHG(State8080 *state);
void XRA(State8080 *state, uint8_t var);
void XTHL(State8080 *state);

#endif
