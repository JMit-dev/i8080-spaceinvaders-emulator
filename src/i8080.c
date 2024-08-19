#include <stdio.h>
#include <stdlib.h>

#include "i8080.h"
#include "opcodes.h"
#include "disassembler.h"

void unimplementedInstruction(State8080* state) {     
	printf("Error: Unimplemented instruction\n");
	state->pc--;
	disassemble8080(state->memory, state->pc);
	printf("\n");
	exit(1);
}

void GenerateInterrupt(State8080* state, int interruptNumber) {
    uint16_t pc = state->pc;
    PUSH(state, (pc & 0xFF00) >> 8, pc & 0xFF);
    state->pc = 8 * interruptNumber;
    state->int_enable = 0;
}


int emulate8080(State8080* state, uint8_t (*readPort)(uint8_t), void (*writePort)(uint8_t, uint8_t)) {
    uint8_t *opcode = &state->memory[state->pc];
    disassemble8080(state->memory, state->pc);

    state->pc += 1;

    switch (*opcode) {
        case 0x00: break;  // NOP

        case 0x01: {  // LXI B, D16
            LXI(&state->b, &state->c, opcode);
            state->pc += 2;
        } break;

        case 0x02: {  // STAX B
            uint16_t address = (state->b << 8) | state->c;
            state->memory[address] = state->a;
        } break;

        case 0x03: INX(&state->b, &state->c); break;  // INX B

        case 0x04: INR(state, &state->b); break;  // INR B

        case 0x05: DCR(state, &state->b); break;  // DCR B

        case 0x06: {  // MVI B, D8
            state->b = opcode[1];
            state->pc += 1;
        } break;

        case 0x07: RLC(state); break;  // RLC

        case 0x08: unimplementedInstruction(state); break;

        case 0x09: {  // DAD B
            uint32_t bc = (state->b << 8) | state->c;
            DAD(state, bc);
        } break;

        case 0x0A: LDAX(state, &state->b, &state->c); break;  // LDAX B

        case 0x0B: DCX(&state->b, &state->c); break;  // DCX B

        case 0x0C: INR(state, &state->c); break;  // INR C

        case 0x0D: DCR(state, &state->c); break;  // DCR C

        case 0x0E: {  // MVI C, D8
            state->c = opcode[1];
            state->pc += 1;
        } break;

        case 0x0F: RRC(state); break;  // RRC

        case 0x10: unimplementedInstruction(state); break;

        case 0x11: {  // LXI D, D16
            LXI(&state->d, &state->e, opcode);
            state->pc += 2;
        } break;

        case 0x12: {  // STAX D
            uint16_t address = (state->d << 8) | state->e;
            state->memory[address] = state->a;
        } break;

        case 0x13: INX(&state->d, &state->e); break;  // INX D

        case 0x14: INR(state, &state->d); break;  // INR D

        case 0x15: DCR(state, &state->d); break;  // DCR D

        case 0x16: {  // MVI D, D8
            state->d = opcode[1];
            state->pc += 1;
        } break;

        case 0x17: RAL(state); break;  // RAL

        case 0x18: unimplementedInstruction(state); break;

        case 0x19: {  // DAD D
            uint32_t de = (state->d << 8) | state->e;
            DAD(state, de); 
        } break;

        case 0x1A: LDAX(state, &state->d, &state->e); break;  // LDAX D

        case 0x1B: DCX(&state->d, &state->e); break;  // DCX D

        case 0x1C: INR(state, &state->e); break;  // INR E

        case 0x1D: DCR(state, &state->e); break;  // DCR E

        case 0x1E: {  // MVI E, D8
            state->e = opcode[1];
            state->pc += 1;
        } break;

        case 0x1F: RAR(state); break;  // RAR

        case 0x20: unimplementedInstruction(state); break;

        case 0x21: {  // LXI H, D16
            LXI(&state->h, &state->l, opcode);
            state->pc += 2;
        } break;

        case 0x22: {  // SHLD adr
            SHLD(state, opcode);
            state->pc += 2;
        } break;

        case 0x23: INX(&state->h, &state->l); break;  // INX H

        case 0x24: INR(state, &state->h); break;  // INR H

        case 0x25: DCR(state, &state->h); break;  // DCR H

        case 0x26: {  // MVI H, D8
            state->h = opcode[1];
            state->pc += 1;
        } break;

        case 0x27: DAA(state); break;  // DAA
        
        case 0x28: unimplementedInstruction(state); break;

        case 0x29: {  // DAD H
            uint32_t hl = (state->h << 8) | state->l;
            DAD(state, hl);
        } break;

        case 0x2A: {  // LHLD adr
            LHLD(state, opcode);
            state->pc += 2;
        } break;

        case 0x2B: DCX(&state->h, &state->l); break;  // DCX H

        case 0x2C: INR(state, &state->l); break;  // INR L

        case 0x2D: DCR(state, &state->l); break;  // DCR L

        case 0x2E: {  // MVI L, D8
            state->l = opcode[1];
            state->pc += 1;
        } break;

        case 0x2F: CMA(state); break;  //CMA (not) 

        case 0x30: unimplementedInstruction(state); break;

        case 0x31: {  // LXI SP, D16
            state->sp = (opcode[2] << 8) | opcode[1];
            state->pc += 2;
        } break;

        case 0x32: {  // STA adr
            STA(state, opcode);
        } break;

        case 0x33: state->sp++; break;  // INX SP

        case 0x34: {  // INR M
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] += 1;
            arithFlags(state, state->memory[offset]);
        } break;

        case 0x35: {  // DCR M
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] -= 1;
            arithFlags(state, state->memory[offset]);
        } break;

        case 0x36: {  // MVI M, D8
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = opcode[1];
            state->pc += 1;            
        } break;

        case 0x37: state->cc.cy = 1; break;  // STC

        case 0x38: unimplementedInstruction(state); break;

        case 0x39: {  // DAD SP
            DAD(state, state->sp);
        } break;

        case 0x3A: {  // LDA adr
            LDA(state, opcode);
            state->pc += 2;
        } break;

        case 0x3B: state->sp--; break;

        case 0x3C: INR(state, &state->a); break;  // INR A

        case 0x3D: DCR(state, &state->a); break;  // DCR A

        case 0x3E: {  // MVI A, D8
            state->a = opcode[1];
            state->pc += 1;
        } break;

        case 0x3F: CMC(state); break;  // CMC

        case 0x40: state->b = state->b; break;  // MOV B,B

        case 0x41: state->b = state->c; break;  // MOV B,C

        case 0x42: state->b = state->d; break;  // MOV B,D

        case 0x43: state->b = state->e; break;  // MOV B,D

        case 0x44: state->b = state->h; break;  // MOV B,H

        case 0x45: state->b = state->l; break;  // MOV B,L

        case 0x46: {  // MOV B,M
            uint16_t offset = (state->h << 8) | state->l;
            state->b = state->memory[offset];
        } break;

        case 0x47: state->b = state->a; break;  // MOV B,A

        case 0x48: state->c = state->b; break;  // MOV C,B

        case 0x49: state->c = state->c; break;  // MOV C,C

        case 0x4A: state->c = state->d; break;  // MOV C,D

        case 0x4B: state->c = state->e; break;  // MOV C,E

        case 0x4C: state->c = state->h; break;  // MOV C,H

        case 0x4D: state->c = state->l; break;  // MOV C,L

        case 0x4E: {  // MOV C,M
            uint16_t offset = (state->h << 8) | state->l;
            state->c = state->memory[offset];
        } break;

        case 0x4F: state->c = state->a; break;  // MOV C,A

        case 0x50: state->d = state->b; break;  // MOV D,B

        case 0x51: state->d = state->c; break;  // MOV D,C

        case 0x52: state->d = state->d; break;  // MOV D,D

        case 0x53: state->d = state->e; break;  // MOV D,E

        case 0x54: state->d = state->h; break;  // MOV D,H

        case 0x55: state->d = state->l; break;  // MOV D,L

        case 0x56: {  // MOV D, M
            uint16_t offset = (state->h << 8) | state->l;
            state->d = state->memory[offset];
        } break;

        case 0x57: state->d = state->a; break;  // MOV D,A

        case 0x58: state->e = state->b; break;  // MOV E,B

        case 0x59: state->e = state->c; break;  // MOV E,C

        case 0x5A: state->e = state->d; break;  // MOV E,D

        case 0x5B: state->e = state->e; break;  // MOV E,E

        case 0x5C: state->e = state->h; break;  // MOV E,H

        case 0x5D: state->e = state->l; break;  // MOV E,L

        case 0x5E: {  // MOVE E, M
            uint16_t offset = (state->h << 8) | state->l;
            state->e = state->memory[offset];
        } break;

        case 0x5F: state->e = state->a; break;  // MOV E,A

        case 0x60: state->h = state->b; break;  // MOV H,B

        case 0x61: state->h = state->c; break;  // MOV H,C

        case 0x62: state->h = state->d; break;  // MOV H,D

        case 0x63: state->h = state->e; break;  // MOV H,E

        case 0x64: state->h = state->h; break;  // MOV H,H

        case 0x65: state->h = state->l; break;  // MOV H,L

        case 0x66: {  // MOV H, M
            uint16_t offset = (state->h << 8) | state->l;
            state->h = state->memory[offset];
        } break;


        case 0x67: state->h = state->a; break;  // MOV H,A

        case 0x68: state->l = state->b; break;  // MOV L,B

        case 0x69: state->l = state->c; break;  // MOV L,C

        case 0x6A: state->l = state->d; break;  // MOV L,D

        case 0x6B: state->l = state->e; break;  // MOV L,E

        case 0x6C: state->l = state->h; break;  // MOV L,H

        case 0x6D: state->l = state->l; break;  // MOV L,L

        case 0x6E: {  // MOV L, M
            uint16_t offset = (state->h << 8) | state->l;
            state->l = state->memory[offset];
        } break;

        case 0x6F: state->l = state->a; break;  // MOV L, A

        case 0x70: {  // MOV M,B
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->b;
        } break;

        case 0x71: {  // MOV M,C
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->c;
        } break;

        case 0x72: {  // MOV M,D
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->d;
        } break;

        case 0x73: {  // MOV M,E
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->e;
        } break;

        case 0x74: {  // MOV M,H
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->h;
        } break;

        case 0x75: {  // MOV M,L
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->l;
        } break;

        case 0x76: {  // HLT
            printf("HLT instruction executed, halting CPU\n");
            exit(0);
        } break;

        case 0x77: {  // MOV M, A
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->a;
        } break;

        case 0x78: state->a = state->b; break;  // MOV A,B

        case 0x79: state->a = state->c; break;  // MOV A,C

        case 0x7A: state->a = state->d; break;  // MOV A, D

        case 0x7B: state->a = state->e; break;  // MOV A, E

        case 0x7C: state->a = state->h; break;  // MOV A, H

        case 0x7D: state->a = state->l; break;  // MOV A, L

        case 0x7E: {  // MOV A, M
            uint16_t offset = (state->h << 8) | state->l;
            state->a = state->memory[offset];
        } break;

        case 0x7F: state->a = state->a; break;  // MOV A,A
        
        case 0x80: ADD(state, state->b); break;  // ADD B

        case 0x81: ADD(state, state->c); break;  // ADD C

        case 0x82: ADD(state, state->d); break;  // ADD D

        case 0x83: ADD(state, state->e); break;  // ADD E

        case 0x84: ADD(state, state->h); break;  // ADD H

        case 0x85: ADD(state, state->l); break;  // ADD L

        case 0x86: {  // ADD M
            uint16_t offset = (state->h << 8) | (state->l);
            ADD(state, state->memory[offset]);
        } break;

        case 0x87: ADD(state, state->a); break;  // ADD A

        case 0x88: ADC(state, state->b); break;  // ADC B

        case 0x89: ADC(state, state->c); break;  // ADC C

        case 0x8A: ADC(state, state->d); break;  // ADC D

        case 0x8B: ADC(state, state->e); break;  // ADC E

        case 0x8C: ADC(state, state->h); break;  // ADC H

        case 0x8D: ADC(state, state->l); break;  // ADC L

        case 0x8E: {  // ADC M
            uint16_t offset = (state->h << 8) | state->l;
            ADC(state, state->memory[offset]);
        } break;

        case 0x8F: ADC(state, state->a); break;  // ADC A

        case 0x90: SUB(state, state->b); break;  // SUB B

        case 0x91: SUB(state, state->c); break;  // SUB C

        case 0x92: SUB(state, state->d); break;  // SUB D

        case 0x93: SUB(state, state->e); break;  // SUB E

        case 0x94: SUB(state, state->h); break;  // SUB H

        case 0x95: SUB(state, state->l); break;  // SUB L

        case 0x96: {  // SUB M
            uint16_t offset = (state->h << 8) | state->l;
            SUB(state, state->memory[offset]);
        } break;

        case 0x97: SUB(state, state->a); break;  // SUB A

        case 0x98: SBB(state, state->b); break;  // SBB B

        case 0x99: SBB(state, state->c); break;  // SBB C

        case 0x9A: SBB(state, state->d); break;  // SBB D

        case 0x9B: SBB(state, state->e); break;  // SBB E

        case 0x9C: SBB(state, state->h); break;  // SBB H

        case 0x9D: SBB(state, state->l); break;  // SBB L

        case 0x9E: {  // SBB M
            uint16_t offset = (state->h << 8) | state->l;
            SBB(state, state->memory[offset]);
        } break;

        case 0x9F: SBB(state, state->a); break;  // SBB A

        case 0xA0: ANA(state, state->b); break;  // ANA B

        case 0xA1: ANA(state, state->c); break;  // ANA C

        case 0xA2: ANA(state, state->d); break;  // ANA D

        case 0xA3: ANA(state, state->e); break;  // ANA E

        case 0xA4: ANA(state, state->h); break;  // ANA H

        case 0xA5: ANA(state, state->l); break;  // ANA L

        case 0xA6: {  // ANA M
            uint16_t offset = (state->h << 8) | state->l;
            ANA(state, state->memory[offset]);
        } break;

        case 0xA7: ANA(state, state->a); break;  // ANA A

        case 0xA8: XRA(state, state->b); break;  // XRA B

        case 0xA9: XRA(state, state->c); break;  // XRA C

        case 0xAA: XRA(state, state->d); break;  // XRA D

        case 0xAB: XRA(state, state->e); break;  // XRA E

        case 0xAC: XRA(state, state->h); break;  // XRA H

        case 0xAD: XRA(state, state->l); break;  // XRA L

        case 0xAE: {  // XRA M
            uint16_t offset = (state->h << 8) | state->l;
            XRA(state, state->memory[offset]);
        } break;

        case 0xAF: XRA(state, state->a); break;  // XRA A 

        case 0xB0: ORA(state, state->b); break;  // ORA B

        case 0xB1: ORA(state, state->c); break;  // ORA C

        case 0xB2: ORA(state, state->d); break;  // ORA D

        case 0xB3: ORA(state, state->e); break;  // ORA E

        case 0xB4: ORA(state, state->h); break;  // ORA H

        case 0xB5: ORA(state, state->l); break;  // ORA L

        case 0xB6: {  // ORA M
            uint16_t offset = (state->h << 8) | state->l;
            ORA(state, state->memory[offset]);
        } break;

        case 0xB7: ORA(state, state->a); break;  // ORA A

        case 0xB8: CMP(state, state->b); break;  // CMP B

        case 0xB9: CMP(state, state->c); break;  // CMP C

        case 0xBA: CMP(state, state->d); break;  // CMP D

        case 0xBB: CMP(state, state->e); break;  // CMP E

        case 0xBC: CMP(state, state->h); break;  // CMP H

        case 0xBD: CMP(state, state->l); break;  // CMP L

        case 0xBE: {  // CMP M
            uint16_t offset = (state->h << 8) | state->l;
            CMP(state, state->memory[offset]);
        } break;

        case 0xBF: CMP(state, state->a); break;  // CMP A

        case 0xC0: {  // RNZ
            if (state->cc.z == 0) {
                RET(state);
            }
        } break;


        case 0xC1: POP(state, &state->b, &state->c); break;  // POP B

        case 0xC2: {  // JNZ adr
            if (state->cc.z == 0) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xC3: JMP(state, opcode); break;  // JMP adr

        case 0xC4: {  // CNZ adr
            if (state->cc.z == 0) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xC5: PUSH(state, state->b, state->c); break;  // PUSH B

        case 0xC6: {  // ADI D8
            ADD(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xC7: {  // RST 0
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0000;
        } break;
        
        case 0xC8: {  // RZ
            if (state->cc.z == 1) {
                RET(state);
            }
        } break;

        case 0xC9: RET(state); break;  // RET

        case 0xCA: {  // JZ adr
            if (state->cc.z == 1) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xCB: unimplementedInstruction(state); break;

        case 0xCC: {  // CZ adr
            if (state->cc.z == 1) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xCD: CALL(state, opcode); break;  // CALL

        case 0xCE: {  // ACI D8
            ADC(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xCF: {  // RST 1
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0008;
        } break;

        case 0xD0: {  // RNC
            if (state->cc.cy == 0) {
                RET(state);
            }
        } break;

        case 0xD1: POP(state, &state->d, &state->e); break;  // POP D

        case 0xD2: {  // JNC adr
            if (state->cc.cy == 0) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xD3: {  // OUT D8
            uint8_t port = opcode[1];
            writePort(port, state->a);
            state->pc += 1;
        } break;

        case 0xD4: {  // CNC adr
            if (state->cc.cy == 0) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xD5: PUSH(state, state->d, state->e); break;  // PUSH D

        case 0xD6: {  // SUI D8
            SUB(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xD7: {  // RST 2
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0010;
        } break;

        case 0xD8: {  // RC
            if (state->cc.cy == 1) {
                RET(state);
            }
        } break;

        case 0xD9: unimplementedInstruction(state); break;

        case 0xDA: {  // JC adr
            if (state->cc.cy == 1) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xDB: {  // IN
            uint8_t port = opcode[1];
            state->a = readPort(port);
            state->pc += 1;
        } break;

        case 0xDC: {  // CC adr
            if (state->cc.cy == 1) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xDD: unimplementedInstruction(state); break;

        case 0xDE: {  // SBI D8
            SBB(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xDF: {  // RST 3
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0018;
        } break;
        
        case 0xE0: {  // RPO
            if (state->cc.p == 0) {
                RET(state);
            }
        } break;

        case 0xE1: POP(state, &state->h, &state->l); break;  // POP H

        case 0xE2: {  // JPO adr
            if (state->cc.p == 0) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xE3: {  // XTHL
            XTHL(state);
        } break;
        
        case 0xE4: {  // CPO adr
            if (state->cc.p == 0) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xE5: PUSH(state, state->h, state->l); break;  // PUSH H

        case 0xE6: {  // ANI D8
            ANA(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xE7: {  // RST 4
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0020;
        } break;
        
        case 0xE8: {  // RPE
            if (state->cc.p == 1) {
                RET(state);
            }
        } break;

        case 0xE9: {  // PCHL
            state->pc = (state->h << 8) | state->l;
        } break;
        
        case 0xEA: {  // JPE adr
            if (state->cc.p == 1) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xEB: XCHG(state); break;  // XCHG

        case 0xEC: {  // CPE adr
            if (state->cc.p == 1) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xED: unimplementedInstruction(state); break;
        
        case 0xEE: {  // XRI D8
            XRA(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xEF: {  // RST 5
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0028;
        } break;

        case 0xF0: {  // RP
            if (state->cc.s == 0) {
                RET(state);
            }
        } break;

        case 0xF1: POP_PSW(state); break;  // POP PSW

        case 0xF2: {  // JP adr
            if (state->cc.s == 0) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xF3: state->int_enable = 0; break;  // DI

        case 0xF4: {  // CP adr
            if (state->cc.s == 0) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xF5: PUSH_PSW(state); break;  // PUSH PSW

        case 0xF6: {  // ORI D8
            ORA(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xF7: {  // RST 6
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0030;
        } break;

        
        case 0xF8: {  // RM
            if (state->cc.s == 1) {
                RET(state);
            }
        } break;

        case 0xF9: {  // SLP
            state->sp = (state->h << 8) | state->l;
        } break;
        
        case 0xFA: {  // JM adr
            if (state->cc.s == 1) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xFB: state->int_enable = 1; break;  // EI

        case 0xFC: {  // CM adr
            if (state->cc.s == 1) {
                CALL(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xFD: unimplementedInstruction(state); break;

        case 0xFE: {  // CPI D8
            CMP(state, opcode[1]); 
            state->pc += 1;
        } break;

        case 0xFF: {  // RST 7
            uint16_t returnAddress = state->pc;
            state->memory[state->sp - 1] = (returnAddress >> 8) & 0xFF;
            state->memory[state->sp - 2] = returnAddress & 0xFF;
            state->sp -= 2;
            state->pc = 0x0038;
        } break;


        default: unimplementedInstruction(state); break;
    }
    printf("\tC=%d,P=%d,S=%d,Z=%d\n", state->cc.cy, state->cc.p,    
            state->cc.s, state->cc.z);    
    printf("\tA $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n",    
            state->a, state->b, state->c, state->d,    
            state->e, state->h, state->l, state->sp);

    return 0;
}
