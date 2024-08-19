#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "SDL2/SDL.h"
#include "emulator.h"
#include "memory.h"
#include "io.h"

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
    uint32_t lastInterruptTime = SDL_GetTicks();

    while (!done) {
        handleInput();
        done = emulate8080(emulator->state, readPort, writePort);

        uint32_t currentTime = SDL_GetTicks();
        if (currentTime - lastInterruptTime >= 16) {  // ~16ms = 1/60th of a second
            if (emulator->state->int_enable) {
                GenerateInterrupt(emulator->state, 2);  // not tested
                lastInterruptTime = currentTime;
            }
        }
    }
}

void freeEmulator(Emulator* emulator) {
    freeMemory(emulator->state);
    free(emulator->state);
}
