#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembler.h"

int disassemble8080(uint8_t *codeBuffer, uint16_t pc, FILE* output) {
    uint8_t *code = &codeBuffer[pc];
    int opbytes = 1;
    fprintf(output, "%04x ", pc);

    switch (*code) {
        case 0x00: fprintf(output, "NOP"); break;
        case 0x01: fprintf(output, "LXI    B,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x02: fprintf(output, "STAX   B"); break;
        case 0x03: fprintf(output, "INX    B"); break;
        case 0x04: fprintf(output, "INR    B"); break;
        case 0x05: fprintf(output, "DCR    B"); break;
        case 0x06: fprintf(output, "MVI    B,#$%02x", code[1]); opbytes = 2; break;
        case 0x07: fprintf(output, "RLC"); break;
        case 0x08: fprintf(output, "NOP"); break; // Officially this is not used, but treated as NOP
        case 0x09: fprintf(output, "DAD    B"); break;
        case 0x0a: fprintf(output, "LDAX   B"); break;
        case 0x0b: fprintf(output, "DCX    B"); break;
        case 0x0c: fprintf(output, "INR    C"); break;
        case 0x0d: fprintf(output, "DCR    C"); break;
        case 0x0e: fprintf(output, "MVI    C,#$%02x", code[1]); opbytes = 2; break;
        case 0x0f: fprintf(output, "RRC"); break;
        case 0x10: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x11: fprintf(output, "LXI    D,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x12: fprintf(output, "STAX   D"); break;
        case 0x13: fprintf(output, "INX    D"); break;
        case 0x14: fprintf(output, "INR    D"); break;
        case 0x15: fprintf(output, "DCR    D"); break;
        case 0x16: fprintf(output, "MVI    D,#$%02x", code[1]); opbytes = 2; break;
        case 0x17: fprintf(output, "RAL"); break;
        case 0x18: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x19: fprintf(output, "DAD    D"); break;
        case 0x1a: fprintf(output, "LDAX   D"); break;
        case 0x1b: fprintf(output, "DCX    D"); break;
        case 0x1c: fprintf(output, "INR    E"); break;
        case 0x1d: fprintf(output, "DCR    E"); break;
        case 0x1e: fprintf(output, "MVI    E,#$%02x", code[1]); opbytes = 2; break;
        case 0x1f: fprintf(output, "RAR"); break;
        case 0x20: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x21: fprintf(output, "LXI    H,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x22: fprintf(output, "SHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x23: fprintf(output, "INX    H"); break;
        case 0x24: fprintf(output, "INR    H"); break;
        case 0x25: fprintf(output, "DCR    H"); break;
        case 0x26: fprintf(output, "MVI    H,#$%02x", code[1]); opbytes = 2; break;
        case 0x27: fprintf(output, "DAA"); break;
        case 0x28: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x29: fprintf(output, "DAD    H"); break;
        case 0x2a: fprintf(output, "LHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x2b: fprintf(output, "DCX    H"); break;
        case 0x2c: fprintf(output, "INR    L"); break;
        case 0x2d: fprintf(output, "DCR    L"); break;
        case 0x2e: fprintf(output, "MVI    L,#$%02x", code[1]); opbytes = 2; break;
        case 0x2f: fprintf(output, "CMA"); break;
        case 0x30: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x31: fprintf(output, "LXI    SP,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x32: fprintf(output, "STA    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x33: fprintf(output, "INX    SP"); break;
        case 0x34: fprintf(output, "INR    M"); break;
        case 0x35: fprintf(output, "DCR    M"); break;
        case 0x36: fprintf(output, "MVI    M,#$%02x", code[1]); opbytes = 2; break;
        case 0x37: fprintf(output, "STC"); break;
        case 0x38: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0x39: fprintf(output, "DAD    SP"); break;
        case 0x3a: fprintf(output, "LDA    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x3b: fprintf(output, "DCX    SP"); break;
        case 0x3c: fprintf(output, "INR    A"); break;
        case 0x3d: fprintf(output, "DCR    A"); break;
        case 0x3e: fprintf(output, "MVI    A,#$%02x", code[1]); opbytes = 2; break;
        case 0x3f: fprintf(output, "CMC"); break;

        // MOV instructions
        case 0x40: fprintf(output, "MOV    B,B"); break;
        case 0x41: fprintf(output, "MOV    B,C"); break;
        case 0x42: fprintf(output, "MOV    B,D"); break;
        case 0x43: fprintf(output, "MOV    B,E"); break;
        case 0x44: fprintf(output, "MOV    B,H"); break;
        case 0x45: fprintf(output, "MOV    B,L"); break;
        case 0x46: fprintf(output, "MOV    B,M"); break;
        case 0x47: fprintf(output, "MOV    B,A"); break;
        case 0x48: fprintf(output, "MOV    C,B"); break;
        case 0x49: fprintf(output, "MOV    C,C"); break;
        case 0x4a: fprintf(output, "MOV    C,D"); break;
        case 0x4b: fprintf(output, "MOV    C,E"); break;
        case 0x4c: fprintf(output, "MOV    C,H"); break;
        case 0x4d: fprintf(output, "MOV    C,L"); break;
        case 0x4e: fprintf(output, "MOV    C,M"); break;
        case 0x4f: fprintf(output, "MOV    C,A"); break;
        case 0x50: fprintf(output, "MOV    D,B"); break;
        case 0x51: fprintf(output, "MOV    D,C"); break;
        case 0x52: fprintf(output, "MOV    D,D"); break;
        case 0x53: fprintf(output, "MOV    D,E"); break;
        case 0x54: fprintf(output, "MOV    D,H"); break;
        case 0x55: fprintf(output, "MOV    D,L"); break;
        case 0x56: fprintf(output, "MOV    D,M"); break;
        case 0x57: fprintf(output, "MOV    D,A"); break;
        case 0x58: fprintf(output, "MOV    E,B"); break;
        case 0x59: fprintf(output, "MOV    E,C"); break;
        case 0x5a: fprintf(output, "MOV    E,D"); break;
        case 0x5b: fprintf(output, "MOV    E,E"); break;
        case 0x5c: fprintf(output, "MOV    E,H"); break;
        case 0x5d: fprintf(output, "MOV    E,L"); break;
        case 0x5e: fprintf(output, "MOV    E,M"); break;
        case 0x5f: fprintf(output, "MOV    E,A"); break;
        case 0x60: fprintf(output, "MOV    H,B"); break;
        case 0x61: fprintf(output, "MOV    H,C"); break;
        case 0x62: fprintf(output, "MOV    H,D"); break;
        case 0x63: fprintf(output, "MOV    H,E"); break;
        case 0x64: fprintf(output, "MOV    H,H"); break;
        case 0x65: fprintf(output, "MOV    H,L"); break;
        case 0x66: fprintf(output, "MOV    H,M"); break;
        case 0x67: fprintf(output, "MOV    H,A"); break;
        case 0x68: fprintf(output, "MOV    L,B"); break;
        case 0x69: fprintf(output, "MOV    L,C"); break;
        case 0x6a: fprintf(output, "MOV    L,D"); break;
        case 0x6b: fprintf(output, "MOV    L,E"); break;
        case 0x6c: fprintf(output, "MOV    L,H"); break;
        case 0x6d: fprintf(output, "MOV    L,L"); break;
        case 0x6e: fprintf(output, "MOV    L,M"); break;
        case 0x6f: fprintf(output, "MOV    L,A"); break;
        case 0x70: fprintf(output, "MOV    M,B"); break;
        case 0x71: fprintf(output, "MOV    M,C"); break;
        case 0x72: fprintf(output, "MOV    M,D"); break;
        case 0x73: fprintf(output, "MOV    M,E"); break;
        case 0x74: fprintf(output, "MOV    M,H"); break;
        case 0x75: fprintf(output, "MOV    M,L"); break;
        case 0x76: fprintf(output, "HLT"); break;
        case 0x77: fprintf(output, "MOV    M,A"); break;
        case 0x78: fprintf(output, "MOV    A,B"); break;
        case 0x79: fprintf(output, "MOV    A,C"); break;
        case 0x7a: fprintf(output, "MOV    A,D"); break;
        case 0x7b: fprintf(output, "MOV    A,E"); break;
        case 0x7c: fprintf(output, "MOV    A,H"); break;
        case 0x7d: fprintf(output, "MOV    A,L"); break;
        case 0x7e: fprintf(output, "MOV    A,M"); break;
        case 0x7f: fprintf(output, "MOV    A,A"); break;


        // ADD instructions
        case 0x80: fprintf(output, "ADD    B"); break;
        case 0x81: fprintf(output, "ADD    C"); break;
        case 0x82: fprintf(output, "ADD    D"); break;
        case 0x83: fprintf(output, "ADD    E"); break;
        case 0x84: fprintf(output, "ADD    H"); break;
        case 0x85: fprintf(output, "ADD    L"); break;
        case 0x86: fprintf(output, "ADD    M"); break;
        case 0x87: fprintf(output, "ADD    A"); break;
        case 0x88: fprintf(output, "ADC    B"); break;
        case 0x89: fprintf(output, "ADC    C"); break;
        case 0x8a: fprintf(output, "ADC    D"); break;
        case 0x8b: fprintf(output, "ADC    E"); break;
        case 0x8c: fprintf(output, "ADC    H"); break;
        case 0x8d: fprintf(output, "ADC    L"); break;
        case 0x8e: fprintf(output, "ADC    M"); break;
        case 0x8f: fprintf(output, "ADC    A"); break;

        // SUB instructions
        case 0x90: fprintf(output, "SUB    B"); break;
        case 0x91: fprintf(output, "SUB    C"); break;
        case 0x92: fprintf(output, "SUB    D"); break;
        case 0x93: fprintf(output, "SUB    E"); break;
        case 0x94: fprintf(output, "SUB    H"); break;
        case 0x95: fprintf(output, "SUB    L"); break;
        case 0x96: fprintf(output, "SUB    M"); break;
        case 0x97: fprintf(output, "SUB    A"); break;
        case 0x98: fprintf(output, "SBB    B"); break;
        case 0x99: fprintf(output, "SBB    C"); break;
        case 0x9a: fprintf(output, "SBB    D"); break;
        case 0x9b: fprintf(output, "SBB    E"); break;
        case 0x9c: fprintf(output, "SBB    H"); break;
        case 0x9d: fprintf(output, "SBB    L"); break;
        case 0x9e: fprintf(output, "SBB    M"); break;
        case 0x9f: fprintf(output, "SBB    A"); break;

        // Logical Instructions
        case 0xa0: fprintf(output, "ANA    B"); break;
        case 0xa1: fprintf(output, "ANA    C"); break;
        case 0xa2: fprintf(output, "ANA    D"); break;
        case 0xa3: fprintf(output, "ANA    E"); break;
        case 0xa4: fprintf(output, "ANA    H"); break;
        case 0xa5: fprintf(output, "ANA    L"); break;
        case 0xa6: fprintf(output, "ANA    M"); break;
        case 0xa7: fprintf(output, "ANA    A"); break;
        case 0xa8: fprintf(output, "XRA    B"); break;
        case 0xa9: fprintf(output, "XRA    C"); break;
        case 0xaa: fprintf(output, "XRA    D"); break;
        case 0xab: fprintf(output, "XRA    E"); break;
        case 0xac: fprintf(output, "XRA    H"); break;
        case 0xad: fprintf(output, "XRA    L"); break;
        case 0xae: fprintf(output, "XRA    M"); break;
        case 0xaf: fprintf(output, "XRA    A"); break;

        // OR and Compare Instructions
        case 0xb0: fprintf(output, "ORA    B"); break;
        case 0xb1: fprintf(output, "ORA    C"); break;
        case 0xb2: fprintf(output, "ORA    D"); break;
        case 0xb3: fprintf(output, "ORA    E"); break;
        case 0xb4: fprintf(output, "ORA    H"); break;
        case 0xb5: fprintf(output, "ORA    L"); break;
        case 0xb6: fprintf(output, "ORA    M"); break;
        case 0xb7: fprintf(output, "ORA    A"); break;
        case 0xb8: fprintf(output, "CMP    B"); break;
        case 0xb9: fprintf(output, "CMP    C"); break;
        case 0xba: fprintf(output, "CMP    D"); break;
        case 0xbb: fprintf(output, "CMP    E"); break;
        case 0xbc: fprintf(output, "CMP    H"); break;
        case 0xbd: fprintf(output, "CMP    L"); break;
        case 0xbe: fprintf(output, "CMP    M"); break;
        case 0xbf: fprintf(output, "CMP    A"); break;

        // Branching Instructions
        case 0xc0: fprintf(output, "RNZ"); break;
        case 0xc1: fprintf(output, "POP    B"); break;
        case 0xc2: fprintf(output, "JNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc3: fprintf(output, "JMP    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc4: fprintf(output, "CNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc5: fprintf(output, "PUSH   B"); break;
        case 0xc6: fprintf(output, "ADI    #$%02x", code[1]); opbytes = 2; break;
        case 0xc7: fprintf(output, "RST    0"); break;
        case 0xc8: fprintf(output, "RZ"); break;
        case 0xc9: fprintf(output, "RET"); break;
        case 0xca: fprintf(output, "JZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcb: fprintf(output, "JMP    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcc: fprintf(output, "CZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcd: fprintf(output, "CALL   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xce: fprintf(output, "ACI    #$%02x", code[1]); opbytes = 2; break;
        case 0xcf: fprintf(output, "RST    1"); break;

        case 0xd0: fprintf(output, "RNC"); break;
        case 0xd1: fprintf(output, "POP    D"); break;
        case 0xd2: fprintf(output, "JNC    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd3: fprintf(output, "OUT    #$%02x", code[1]); opbytes = 2; break;
        case 0xd4: fprintf(output, "CNC    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd5: fprintf(output, "PUSH   D"); break;
        case 0xd6: fprintf(output, "SUI    #$%02x", code[1]); opbytes = 2; break;
        case 0xd7: fprintf(output, "RST    2"); break;
        case 0xd8: fprintf(output, "RC"); break;
        case 0xd9: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0xda: fprintf(output, "JC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdb: fprintf(output, "IN     #$%02x", code[1]); opbytes = 2; break;
        case 0xdc: fprintf(output, "CC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdd: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0xde: fprintf(output, "SBI    #$%02x", code[1]); opbytes = 2; break;
        case 0xdf: fprintf(output, "RST    3"); break;

        case 0xe0: fprintf(output, "RPO"); break;
        case 0xe1: fprintf(output, "POP    H"); break;
        case 0xe2: fprintf(output, "JPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe3: fprintf(output, "XTHL"); break;
        case 0xe4: fprintf(output, "CPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe5: fprintf(output, "PUSH   H"); break;
        case 0xe6: fprintf(output, "ANI    #$%02x", code[1]); opbytes = 2; break;
        case 0xe7: fprintf(output, "RST    4"); break;
        case 0xe8: fprintf(output, "RPE"); break;
        case 0xe9: fprintf(output, "PCHL"); break;
        case 0xea: fprintf(output, "JPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xeb: fprintf(output, "XCHG"); break;
        case 0xec: fprintf(output, "CPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xed: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0xee: fprintf(output, "XRI    #$%02x", code[1]); opbytes = 2; break;
        case 0xef: fprintf(output, "RST    5"); break;

        case 0xf0: fprintf(output, "RP"); break;
        case 0xf1: fprintf(output, "POP    PSW"); break;
        case 0xf2: fprintf(output, "JP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf3: fprintf(output, "DI"); break;
        case 0xf4: fprintf(output, "CP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf5: fprintf(output, "PUSH   PSW"); break;
        case 0xf6: fprintf(output, "ORI    #$%02x", code[1]); opbytes = 2; break;
        case 0xf7: fprintf(output, "RST    6"); break;
        case 0xf8: fprintf(output, "RM"); break;
        case 0xf9: fprintf(output, "SPHL"); break;
        case 0xfa: fprintf(output, "JM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfb: fprintf(output, "EI"); break;
        case 0xfc: fprintf(output, "CM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfd: fprintf(output, "NOP"); break; // Unofficial NOP
        case 0xfe: fprintf(output, "CPI    #$%02x", code[1]); opbytes = 2; break;
        case 0xff: fprintf(output, "RST    7"); break;


        default: fprintf(output, "UNKNOWN"); break;
    }

    fprintf(output, "\n");

    return opbytes;
}

void disassembleROM(const char *filename) {
    FILE *rom = fopen(filename, "rb");
    if (rom == NULL) {
        fprintf(stderr, "Error: Couldn't open %s\n", filename);
        exit(1);
    }

    fseek(rom, 0, SEEK_END);
    int romSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(romSize);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Couldn't allocate memory\n");
        fclose(rom);
        exit(1);
    }

    fread(buffer, romSize, 1, rom);
    fclose(rom);

    int pc = 0;
    while (pc < romSize) {
        pc += disassemble8080(buffer, pc, stdout);
    }

    free(buffer);
}

static void disassemble_file(const char *filename, uint16_t base_addr, FILE* output) {
    FILE *rom = fopen(filename, "rb");
    if (rom == NULL) {
        fprintf(stderr, "Error: Couldn't open %s\n", filename);
        return;
    }

    fseek(rom, 0, SEEK_END);
    int romSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(romSize);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Couldn't allocate memory\n");
        fclose(rom);
        return;
    }

    fread(buffer, romSize, 1, rom);
    fclose(rom);

    // Create a larger buffer to hold the ROM at the correct address
    unsigned char *full_buffer = (unsigned char *)calloc(0x10000, 1);
    memcpy(full_buffer + base_addr, buffer, romSize);

    int pc = base_addr;
    int end = base_addr + romSize;

    // Disassemble to the output file
    while (pc < end) {
        pc += disassemble8080(full_buffer, pc, output);
    }

    free(full_buffer);
    free(buffer);
}

void disassemble_space_invaders(FILE* output) {
    fprintf(output, "=== Space Invaders ROM Disassembly ===\n\n");
    fprintf(output, "--- invaders.h (0x0000-0x07FF) ---\n");
    disassemble_file("./rsrc/invaders.h", 0x0000, output);
    fprintf(output, "\n--- invaders.g (0x0800-0x0FFF) ---\n");
    disassemble_file("./rsrc/invaders.g", 0x0800, output);
    fprintf(output, "\n--- invaders.f (0x1000-0x17FF) ---\n");
    disassemble_file("./rsrc/invaders.f", 0x1000, output);
    fprintf(output, "\n--- invaders.e (0x1800-0x1FFF) ---\n");
    disassemble_file("./rsrc/invaders.e", 0x1800, output);
}