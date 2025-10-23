#include "opcodes.h"

int parity(uint8_t value) {
    int32_t count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return !(count & 1);
}

void logicFlags(State8080 *state) {
    state->cc.z = (state->a == 0);
    state->cc.s = (0x80 == (state->a & 0x80));
    state->cc.p = parity(state->a);
    state->cc.cy = 0;
    state->cc.ac = 0;
}

void arithFlags(State8080 *state, uint16_t res) {
    state->cc.z = ((res & 0xFF) == 0);
    state->cc.s = (0x80 == (res & 0x80));
    state->cc.p = parity(res & 0xFF);
    state->cc.cy = (res > 0xFF);
}

void bcdArithFlags(State8080 *state, uint16_t res) {
    state->cc.z = ((res & 0xFF) == 0);
    state->cc.s = (0x80 == (res & 0x80));
    state->cc.p = parity(res & 0xFF);
    state->cc.cy = (res > 0xFF);
    state->cc.ac = (res > 0x09);
}

void ADD(State8080 *state, uint8_t addend) {
    uint16_t result = (uint16_t)state->a + (uint16_t)addend;
    arithFlags(state, result);
    state->a = result & 0xFF;
}

void ADC(State8080 *state, uint8_t addend) {
    uint16_t result = (uint16_t)state->a + (uint16_t)addend + (uint16_t)state->cc.cy;
    arithFlags(state, result);
    state->a = result & 0xFF;
}

void SUB(State8080 *state, uint8_t subtrahend) {
    uint16_t result = (uint16_t)state->a - (uint16_t)subtrahend;
    arithFlags(state, result);
    state->a = result & 0xFF;
}

void SBB(State8080 *state, uint8_t subtrahend) {
    uint16_t result = (uint16_t)state->a - (uint16_t)subtrahend - (uint16_t)state->cc.cy;
    arithFlags(state, result);
    state->a = result & 0xFF;
}

void ANA(State8080 *state, uint8_t var) {
    state->a = state->a & var;
    logicFlags(state);
}

void ORA(State8080 *state, uint8_t var) {
    state->a = state->a | var;
    logicFlags(state);
}

void XRA(State8080 *state, uint8_t var) {
    state->a = state->a ^ var;
    logicFlags(state);
}

void CMP(State8080 *state, uint8_t var) {
    uint16_t result = state->a - var;
    state->cc.z = (result == 0);
    state->cc.s = (0x80 == (result & 0x80));
    state->cc.p = parity(result);
    state->cc.cy = (state->a < var);
}

void LXI(uint8_t *msbReg, uint8_t *lsbReg, uint8_t *opcode) {
    *lsbReg = opcode[1];
    *msbReg = opcode[2];
}

void LDA(State8080 *state, uint8_t *opcode) {
    uint16_t address = (opcode[2] << 8) | opcode[1];
    state->a = state->memory[address];
}

void STA(State8080 *state, uint8_t *opcode) {
    uint16_t address = (opcode[2] << 8) | opcode[1];
    state->memory[address] = state->a;
    state->pc += 2;
}

void LHLD(State8080 *state, uint8_t *opcode) {
    uint16_t memLocation = (opcode[2] << 8) | opcode[1];
    state->l = state->memory[memLocation];
    state->h = state->memory[memLocation + 1];
}

void SHLD(State8080 *state, uint8_t *opcode) {
    uint16_t memLocation = (opcode[2] << 8) | opcode[1];
    state->memory[memLocation] = state->l;
    state->memory[memLocation + 1] = state->h;
}

void PUSH(State8080 *state, uint8_t msbReg, uint8_t lsbReg) {
    state->memory[state->sp - 1] = msbReg;
    state->memory[state->sp - 2] = lsbReg;
    state->sp -= 2;
}

void POP(State8080 *state, uint8_t *msbReg, uint8_t *lsbReg) {
    *lsbReg = state->memory[state->sp];
    *msbReg = state->memory[state->sp + 1];
    state->sp += 2;
}

void PUSH_PSW(State8080 *state) {
    uint8_t psw = (state->cc.z << 6) | (state->cc.s << 7) | (state->cc.p << 2) | (state->cc.cy) | 0x02;
    state->memory[state->sp - 1] = state->a;
    state->memory[state->sp - 2] = psw;
    state->sp -= 2;
}

void POP_PSW(State8080 *state) {
    uint8_t psw = state->memory[state->sp];
    state->a = state->memory[state->sp + 1];
    state->cc.z = (psw >> 6) & 1;
    state->cc.s = (psw >> 7) & 1;
    state->cc.p = (psw >> 2) & 1;
    state->cc.cy = psw & 1;
    state->sp += 2;
}

void JMP(State8080 *state, uint8_t *opcode) {
    state->pc = (opcode[2] << 8) | opcode[1];
}

void CALL(State8080 *state, uint8_t *opcode) {
    uint16_t ret = state->pc + 2;
    state->memory[state->sp - 1] = (ret >> 8) & 0xFF;
    state->memory[state->sp - 2] = (ret & 0xFF);
    state->sp -= 2;
    state->pc = (opcode[2] << 8) | opcode[1];
}

void RET(State8080 *state) {
    state->pc = (state->memory[state->sp + 1] << 8) | state->memory[state->sp];
    state->sp += 2;
}

void RLC(State8080 *state) {
    uint8_t temp = state->a;
    state->a = (temp << 1) | (temp >> 7);
    state->cc.cy = (temp >> 7) & 1;
}

void RRC(State8080 *state) {
    uint8_t temp = state->a;
    state->a = (temp >> 1) | (temp << 7);
    state->cc.cy = temp & 1;
}

void RAL(State8080 *state) {
    uint8_t temp = state->a;
    state->a = (temp << 1) | state->cc.cy;
    state->cc.cy = (temp >> 7) & 1;
}

void RAR(State8080 *state) {
    uint8_t temp = state->a;
    state->a = (temp >> 1) | (state->cc.cy << 7);
    state->cc.cy = temp & 1;
}

void CMA(State8080 *state) {
    state->a = ~state->a;
}

void CMC(State8080 *state) {
    state->cc.cy = ~state->cc.cy;
}

void DAA(State8080 *state) {
    uint8_t lsb = state->a & 0x0F;
    if (lsb > 0x09 || state->cc.ac) {
        state->a += 0x06;
    }
    uint8_t msb = state->a & 0xF0;
    if (msb > 0x90 || state->cc.cy) {
        state->a += 0x60;
        state->cc.cy = 1;
    }
    arithFlags(state, state->a);
}

void INR(State8080 *state, uint8_t *reg) {
    *reg += 1;
    arithFlags(state, *reg);
}

void DAD(State8080 *state, uint32_t regPair) {
	uint32_t hl = (state->h << 8) | state->l;
	uint32_t result = hl + regPair;
	state->cc.cy = ((result & 0xFFFF0000) > 0);
	state->h = (result & 0xFF00) >> 8;
	state->l = (result & 0xFF);
}

void DCR(State8080 *state, uint8_t *reg) {
    *reg -= 1;
    arithFlags(state, *reg);
}

void INX(uint8_t *msbReg, uint8_t *lsbReg) {
    (*lsbReg)++;
    if (*lsbReg == 0) {
        (*msbReg)++;
    }
}

void DCX(uint8_t *msbReg, uint8_t *lsbReg) {
    if (*lsbReg == 0) {
        (*msbReg)--;
    }
    (*lsbReg)--;
}

void XCHG(State8080 *state) {
    uint8_t tempD = state->d;
    uint8_t tempE = state->e;
    state->d = state->h;
    state->e = state->l;
    state->h = tempD;
    state->l = tempE;
}

void XTHL(State8080 *state) {
    uint8_t tempL = state->l;
    uint8_t tempH = state->h;
    state->l = state->memory[state->sp];
    state->h = state->memory[state->sp + 1];
    state->memory[state->sp] = tempL;
    state->memory[state->sp + 1] = tempH;
}

void LDAX(State8080 *state, uint8_t *msbReg, uint8_t *lsbReg) {
    uint16_t address = (*msbReg << 8) | *lsbReg;
    state->a = state->memory[address];
}