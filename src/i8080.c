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

        case 0x02: unimplementedInstruction(state); break;
        case 0x03: unimplementedInstruction(state); break;
        case 0x04: unimplementedInstruction(state); break;

        case 0x05: DCR(state, &state->b); break;  // DCR B

        case 0x06: {  // MVI B, D8
            state->b = opcode[1];
            state->pc += 1;
        } break;

        case 0x07: unimplementedInstruction(state); break;
        case 0x08: unimplementedInstruction(state); break;

        case 0x09: {  // DAD B
            uint32_t bc = (state->b << 8) | state->c;
            DAD(state, bc);
        } break;

        case 0x0A: unimplementedInstruction(state); break;
        case 0x0B: unimplementedInstruction(state); break;
        case 0x0C: unimplementedInstruction(state); break;

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

        case 0x12: unimplementedInstruction(state); break;

        case 0x13: INX(&state->d, &state->e); break;  // INX D

        case 0x14: unimplementedInstruction(state); break;
        case 0x15: unimplementedInstruction(state); break;
        case 0x16: unimplementedInstruction(state); break;
        case 0x17: unimplementedInstruction(state); break;
        case 0x18: unimplementedInstruction(state); break;

        case 0x19: {  // DAD D
            uint32_t de = (state->d << 8) | state->e;
            DAD(state, de); 
        } break;

        case 0x1A: LDAX(state, &state->d, &state->e); break;  // LDAX D

        case 0x1B: unimplementedInstruction(state); break;
        case 0x1C: unimplementedInstruction(state); break;
        case 0x1D: unimplementedInstruction(state); break;
        case 0x1E: unimplementedInstruction(state); break;

        case 0x1F: RAR(state); break;  // RAR

        case 0x20: unimplementedInstruction(state); break;

        case 0x21: {  // LXI H, D16
            LXI(&state->h, &state->l, opcode);
            state->pc += 2;
        } break;

        case 0x22: unimplementedInstruction(state); break;

        case 0x23: INX(&state->h, &state->l); break;  // INX H

        case 0x24: unimplementedInstruction(state); break;
        case 0x25: unimplementedInstruction(state); break;

        case 0x26: {  // MVI H, D8
            state->h = opcode[1];
            state->pc += 1;
        } break;

        case 0x27: unimplementedInstruction(state); break;
        case 0x28: unimplementedInstruction(state); break;

        case 0x29: {  // DAD H
            uint32_t hl = (state->h << 8) | state->l;
            DAD(state, hl);
        } break;

        case 0x2A: unimplementedInstruction(state); break;
        case 0x2B: unimplementedInstruction(state); break;
        case 0x2C: unimplementedInstruction(state); break;
        case 0x2D: unimplementedInstruction(state); break;
        case 0x2E: unimplementedInstruction(state); break;

        case 0x2F: CMA(state); break;  //CMA (not) 

        case 0x30: unimplementedInstruction(state); break;

        case 0x31: {  // LXI SP, D16
            state->sp = (opcode[2] << 8) | opcode[1];
            state->pc += 2;
        } break;

        case 0x32: {  // STA adr
            STA(state, opcode);
        } break;

        case 0x33: unimplementedInstruction(state); break;
        case 0x34: unimplementedInstruction(state); break;
        case 0x35: unimplementedInstruction(state); break;

        case 0x36: {  // MVI M, D8
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = opcode[1];
            state->pc += 1;            
        } break;

        case 0x37: unimplementedInstruction(state); break;
        case 0x38: unimplementedInstruction(state); break;
        case 0x39: unimplementedInstruction(state); break;

        case 0x3A: {  // LDA adr
            LDA(state, opcode);
            state->pc += 2;
        } break;

        case 0x3B: unimplementedInstruction(state); break;
        case 0x3C: unimplementedInstruction(state); break;
        case 0x3D: unimplementedInstruction(state); break;

        case 0x3E: {  // MVI A, D8
            state->a = opcode[1];
            state->pc += 1;
        } break;

        case 0x3F: unimplementedInstruction(state); break;
        case 0x40: unimplementedInstruction(state); break;

        case 0x41: state->b = state->c; break;  // MOV B,C

        case 0x42: state->b = state->d; break;  // MOV B,D

        case 0x43: state->b = state->e; break;  // MOV B,D

        case 0x44: unimplementedInstruction(state); break;
        case 0x45: unimplementedInstruction(state); break;
        case 0x46: unimplementedInstruction(state); break;
        case 0x47: unimplementedInstruction(state); break;
        case 0x48: unimplementedInstruction(state); break;
        case 0x49: unimplementedInstruction(state); break;
        case 0x4A: unimplementedInstruction(state); break;
        case 0x4B: unimplementedInstruction(state); break;
        case 0x4C: unimplementedInstruction(state); break;
        case 0x4D: unimplementedInstruction(state); break;
        case 0x4E: unimplementedInstruction(state); break;
        case 0x4F: unimplementedInstruction(state); break;
        case 0x50: unimplementedInstruction(state); break;
        case 0x51: unimplementedInstruction(state); break;
        case 0x52: unimplementedInstruction(state); break;
        case 0x53: unimplementedInstruction(state); break;
        case 0x54: unimplementedInstruction(state); break;
        case 0x55: unimplementedInstruction(state); break;

        case 0x56: {  // MOV D, M
            uint16_t offset = (state->h << 8) | state->l;
            state->d = state->memory[offset];
        } break;

        case 0x57: unimplementedInstruction(state); break;
        case 0x58: unimplementedInstruction(state); break;
        case 0x59: unimplementedInstruction(state); break;
        case 0x5A: unimplementedInstruction(state); break;
        case 0x5B: unimplementedInstruction(state); break;
        case 0x5C: unimplementedInstruction(state); break;
        case 0x5D: unimplementedInstruction(state); break;

        case 0x5E: {  // MOVE E, M
            uint16_t offset = (state->h << 8) | state->l;
            state->e = state->memory[offset];
        } break;

        case 0x5F: unimplementedInstruction(state); break;
        case 0x60: unimplementedInstruction(state); break;
        case 0x61: unimplementedInstruction(state); break;
        case 0x62: unimplementedInstruction(state); break;
        case 0x63: unimplementedInstruction(state); break;
        case 0x64: unimplementedInstruction(state); break;
        case 0x65: unimplementedInstruction(state); break;

        case 0x66: {  // MOV H, M
            uint16_t offset = (state->h << 8) | state->l;
            state->h = state->memory[offset];
        } break;

        case 0x67: unimplementedInstruction(state); break;
        case 0x68: unimplementedInstruction(state); break;
        case 0x69: unimplementedInstruction(state); break;
        case 0x6A: unimplementedInstruction(state); break;
        case 0x6B: unimplementedInstruction(state); break;
        case 0x6C: unimplementedInstruction(state); break;
        case 0x6D: unimplementedInstruction(state); break;
        case 0x6E: unimplementedInstruction(state); break;

        case 0x6F: state->l = state->a; break;  // MOV L, A

        case 0x70: unimplementedInstruction(state); break;
        case 0x71: unimplementedInstruction(state); break;
        case 0x72: unimplementedInstruction(state); break;
        case 0x73: unimplementedInstruction(state); break;
        case 0x74: unimplementedInstruction(state); break;
        case 0x75: unimplementedInstruction(state); break;

        case 0x76: {  // HLT
            printf("HLT instruction executed, halting CPU\n");
            exit(0);
        } break;

        case 0x77: {  // MOV M, A
            uint16_t offset = (state->h << 8) | state->l;
            state->memory[offset] = state->a;
        } break;

        case 0x78: unimplementedInstruction(state); break;
        case 0x79: unimplementedInstruction(state); break;

        case 0x7A: state->a = state->d; break;  // MOV A, D

        case 0x7B: state->a = state->e; break;  // MOV A, E

        case 0x7C: state->a = state->h; break;  // MOV A, H

        case 0x7D: unimplementedInstruction(state); break;

        case 0x7E: {  // MOV A, M
            uint16_t offset = (state->h << 8) | state->l;
            state->a = state->memory[offset];
        } break;

        case 0x7F: unimplementedInstruction(state); break;
        
        case 0x80: ADD(state, state->b); break;  // ADD B

        case 0x81: ADD(state, state->c); break;  // ADD C

        case 0x82: unimplementedInstruction(state); break;
        case 0x83: unimplementedInstruction(state); break;
        case 0x84: unimplementedInstruction(state); break;
        case 0x85: unimplementedInstruction(state); break;

        case 0x86: {  // ADD M
            uint16_t offset = (state->h << 8) | (state->l);
            ADD(state, state->memory[offset]);
        } break;

        case 0x87: unimplementedInstruction(state); break;
        case 0x88: unimplementedInstruction(state); break;
        case 0x89: unimplementedInstruction(state); break;
        case 0x8A: unimplementedInstruction(state); break;
        case 0x8B: unimplementedInstruction(state); break;
        case 0x8C: unimplementedInstruction(state); break;
        case 0x8D: unimplementedInstruction(state); break;
        case 0x8E: unimplementedInstruction(state); break;
        case 0x8F: unimplementedInstruction(state); break;
        case 0x90: unimplementedInstruction(state); break;
        case 0x91: unimplementedInstruction(state); break;
        case 0x92: unimplementedInstruction(state); break;
        case 0x93: unimplementedInstruction(state); break;
        case 0x94: unimplementedInstruction(state); break;
        case 0x95: unimplementedInstruction(state); break;
        case 0x96: unimplementedInstruction(state); break;
        case 0x97: unimplementedInstruction(state); break;
        case 0x98: unimplementedInstruction(state); break;
        case 0x99: unimplementedInstruction(state); break;
        case 0x9A: unimplementedInstruction(state); break;
        case 0x9B: unimplementedInstruction(state); break;
        case 0x9C: unimplementedInstruction(state); break;
        case 0x9D: unimplementedInstruction(state); break;
        case 0x9E: unimplementedInstruction(state); break;
        case 0x9F: unimplementedInstruction(state); break;
        case 0xA0: unimplementedInstruction(state); break;
        case 0xA1: unimplementedInstruction(state); break;
        case 0xA2: unimplementedInstruction(state); break;
        case 0xA3: unimplementedInstruction(state); break;
        case 0xA4: unimplementedInstruction(state); break;
        case 0xA5: unimplementedInstruction(state); break;
        case 0xA6: unimplementedInstruction(state); break;

        case 0xA7: ANA(state, state->a); break;  // ANA A

        case 0xA8: unimplementedInstruction(state); break;
        case 0xA9: unimplementedInstruction(state); break;
        case 0xAA: unimplementedInstruction(state); break;
        case 0xAB: unimplementedInstruction(state); break;
        case 0xAC: unimplementedInstruction(state); break;
        case 0xAD: unimplementedInstruction(state); break;
        case 0xAE: unimplementedInstruction(state); break;

        case 0xAF: XRA(state, state->a); break;  // XRA A 

        case 0xB0: unimplementedInstruction(state); break;
        case 0xB1: unimplementedInstruction(state); break;
        case 0xB2: unimplementedInstruction(state); break;
        case 0xB3: unimplementedInstruction(state); break;
        case 0xB4: unimplementedInstruction(state); break;
        case 0xB5: unimplementedInstruction(state); break;
        case 0xB6: unimplementedInstruction(state); break;
        case 0xB7: unimplementedInstruction(state); break;
        case 0xB8: unimplementedInstruction(state); break;
        case 0xB9: unimplementedInstruction(state); break;
        case 0xBA: unimplementedInstruction(state); break;
        case 0xBB: unimplementedInstruction(state); break;
        case 0xBC: unimplementedInstruction(state); break;
        case 0xBD: unimplementedInstruction(state); break;
        case 0xBE: unimplementedInstruction(state); break;
        case 0xBF: unimplementedInstruction(state); break;
        case 0xC0: unimplementedInstruction(state); break;

        case 0xC1: POP(state, &state->b, &state->c); break;  // POP B

        case 0xC2: {  // JNZ adr
            if (state->cc.z == 0) {
                JMP(state, opcode);
            } else {
                state->pc += 2;
            }
        } break;

        case 0xC3: JMP(state, opcode); break;  // JMP adr

        case 0xC4: unimplementedInstruction(state); break;

        case 0xC5: PUSH(state, state->b, state->c); break;  // PUSH B

        case 0xC6: {  // ADI D8
            ADD(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xC7: unimplementedInstruction(state); break;
        case 0xC8: unimplementedInstruction(state); break;

        case 0xC9: RET(state); break;  // RET

        case 0xCA: unimplementedInstruction(state); break;
        case 0xCB: unimplementedInstruction(state); break;
        case 0xCC: unimplementedInstruction(state); break;

        case 0xCD: CALL(state, opcode); break;  // CALL

        case 0xCE: unimplementedInstruction(state); break;
        case 0xCF: unimplementedInstruction(state); break;
        case 0xD0: unimplementedInstruction(state); break;

        case 0xD1: POP(state, &state->d, &state->e); break;  // POP D

        case 0xD2: unimplementedInstruction(state); break;

        case 0xD3: {  // OUT D8
            uint8_t port = opcode[1];
            writePort(port, state->a);
            state->pc += 1;
        } break;

        case 0xD4: unimplementedInstruction(state); break;

        case 0xD5: PUSH(state, state->d, state->e); break;  // PUSH D

        case 0xD6: unimplementedInstruction(state); break;
        case 0xD7: unimplementedInstruction(state); break;
        case 0xD8: unimplementedInstruction(state); break;
        case 0xD9: unimplementedInstruction(state); break;
        case 0xDA: unimplementedInstruction(state); break;

        case 0xDB: {  // IN
            uint8_t port = opcode[1];
            state->a = readPort(port);
            state->pc += 1;
        } break;

        case 0xDC: unimplementedInstruction(state); break;
        case 0xDD: unimplementedInstruction(state); break;
        case 0xDE: unimplementedInstruction(state); break;
        case 0xDF: unimplementedInstruction(state); break;
        case 0xE0: unimplementedInstruction(state); break;

        case 0xE1: POP(state, &state->h, &state->l); break;  // POP H

        case 0xE2: unimplementedInstruction(state); break;
        case 0xE3: unimplementedInstruction(state); break;
        case 0xE4: unimplementedInstruction(state); break;

        case 0xE5: PUSH(state, state->h, state->l); break;  // PUSH H

        case 0xE6: {  // ANI D8
            ANA(state, opcode[1]);
            state->pc += 1;
        } break;

        case 0xE7: unimplementedInstruction(state); break;
        case 0xE8: unimplementedInstruction(state); break;
        case 0xE9: unimplementedInstruction(state); break;
        case 0xEA: unimplementedInstruction(state); break;

        case 0xEB: XCHG(state); break;  // XCHG

        case 0xEC: unimplementedInstruction(state); break;
        case 0xED: unimplementedInstruction(state); break;
        case 0xEE: unimplementedInstruction(state); break;
        case 0xEF: unimplementedInstruction(state); break;
        case 0xF0: unimplementedInstruction(state); break;

        case 0xF1: POP_PSW(state); break;  // POP PSW

        case 0xF2: unimplementedInstruction(state); break;

        case 0xF3: state->int_enable = 0; break;  // DI

        case 0xF4: unimplementedInstruction(state); break;

        case 0xF5: PUSH_PSW(state); break;  // PUSH PSW

        case 0xF6: unimplementedInstruction(state); break;
        case 0xF7: unimplementedInstruction(state); break;
        case 0xF8: unimplementedInstruction(state); break;
        case 0xF9: unimplementedInstruction(state); break;
        case 0xFA: unimplementedInstruction(state); break;

        case 0xFB: state->int_enable = 1; break;  // EI

        case 0xFC: unimplementedInstruction(state); break;
        case 0xFD: unimplementedInstruction(state); break;

        case 0xFE: {  // CPI D8
            CMP(state, opcode[1]); 
            state->pc += 1;
        } break;

        case 0xFF: unimplementedInstruction(state); break;

        default: unimplementedInstruction(state); break;
    }
    printf("\tC=%d,P=%d,S=%d,Z=%d\n", state->cc.cy, state->cc.p,    
            state->cc.s, state->cc.z);    
    printf("\tA $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n",    
            state->a, state->b, state->c, state->d,    
            state->e, state->h, state->l, state->sp);

    return 0;
}
