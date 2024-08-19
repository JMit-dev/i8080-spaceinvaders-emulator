#ifndef EMULATOR_H
#define EMULATOR_H

#include "i8080.h"

typedef struct Emulator {
    State8080* state;
} Emulator;

void initializeEmulator(Emulator*, const char*);
void resetEmulator(Emulator*);
void runEmulator(Emulator*);
void freeEmulator(Emulator*);

#endif
