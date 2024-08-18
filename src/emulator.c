#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "emulator.h"
#include "memory.h"

void initializeEmulator(Emulator* emulator, const char* filename) {
    emulator->state = calloc(1, sizeof(State8080));
    initializeMemory(emulator->state, 0x10000);  // 16KB
    loadROM(emulator->state, filename, 0);
}

//  will break for now, need to implement cartridge
void resetEmulator(Emulator* emulator) {
    memset(emulator->state, 0, sizeof(State8080));
    initializeMemory(emulator->state, 0x10000);  // 16KB
    //  need to load cartridge
}

void runEmulator(Emulator* emulator) {
    int done = 0;
    while (!done) {
        done = emulate8080(emulator->state);
    }
}

void freeEmulator(Emulator* emulator) {
    freeMemory(emulator->state);
    free(emulator->state);
}
