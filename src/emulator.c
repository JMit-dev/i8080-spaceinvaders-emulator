#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "SDL2/SDL.h"
#include "emulator.h"
#include "memory.h"
#include "io.h"
#include "display.h"

void initializeEmulator(Emulator* emulator) {
    emulator->state = calloc(1, sizeof(State8080));
    initializeMemory(emulator->state, 0x10000);  // 16KB
    loadROM(emulator->state, "./roms/invaders.h", 0x0000);
    loadROM(emulator->state, "./roms/invaders.g", 0x0800);
    loadROM(emulator->state, "./roms/invaders.f", 0x1000);
    loadROM(emulator->state, "./roms/invaders.e", 0x1800);

    emulator->display = malloc(sizeof(Display));
    initializeDisplay(emulator->display, 224, 256);  // 224x256 display
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
    int whichInterrupt = 1;  // Alternate between RST 1 (mid-frame) and RST 2 (end-frame)

    while (!done) {
        handleInput();
        done = emulate8080(emulator->state, readPort, writePort);

        uint32_t currentTime = SDL_GetTicks();
        if (currentTime - lastInterruptTime >= 8) {  // ~8ms = 1/120th of a second (two interrupts per frame)
            if (emulator->state->int_enable) {
                GenerateInterrupt(emulator->state, whichInterrupt);

                // Only draw on RST 2 (end of frame / VBLANK)
                if (whichInterrupt == 2) {
                    drawDisplay(emulator->display, emulator->state->memory + 0x2400);
                }

                // Alternate between RST 1 and RST 2
                whichInterrupt = (whichInterrupt == 1) ? 2 : 1;
                lastInterruptTime = currentTime;
            }
        }
    }
}

void freeEmulator(Emulator* emulator) {
    destroyDisplay(emulator->display);
    free(emulator->display);
    freeMemory(emulator->state);
    free(emulator->state);
}
