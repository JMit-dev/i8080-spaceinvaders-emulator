#ifndef EMULATOR_H
#define EMULATOR_H

#include "i8080.h"
#include "platform.h"
#include "input.h"
#include "io.h"

// Space Invaders machine emulator
// Ties together CPU, platform, input, and I/O

typedef struct Emulator {
    State8080* cpu;
    Platform* platform;
    Input* input;
    IOState* io;
} Emulator;

typedef enum {
    ROM_SPACE_INVADERS,
    ROM_CPU_DIAG
} ROMType;

Emulator* emulator_create(ROMType rom_type);
void emulator_destroy(Emulator* emu);
void emulator_run(Emulator* emu);
void emulator_run_headless(Emulator* emu);

#endif
