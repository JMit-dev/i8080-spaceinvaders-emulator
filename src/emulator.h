#ifndef EMULATOR_H
#define EMULATOR_H

#include "i8080.h"

typedef struct Emulator {
    State8080* state;
} Emulator;

void initializeEmulator(Emulator* emulator, const char* filename);
void resetEmulator(Emulator* emulator);
void runEmulator(Emulator* emulator);
void freeEmulator(Emulator* emulator);

#endif
