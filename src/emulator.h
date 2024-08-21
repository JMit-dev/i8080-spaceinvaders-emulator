#ifndef EMULATOR_H
#define EMULATOR_H

#include "i8080.h"
#include "display.h"

typedef struct Emulator {
    State8080* state;
    Display* display;
} Emulator;

void initializeEmulator(Emulator*, const char*);
void resetEmulator(Emulator*);
void runEmulator(Emulator*);
void freeEmulator(Emulator*);

#endif
