#include <stdio.h>
#include <string.h>

#define SDL_MAIN_HANDLED

#include "hexdump.h"
#include "disassembler.h"
#include "emulator.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <option> [ROM file]\n", argv[0]);
        printf("Options:\n");
        printf("  run         Run Space Invaders emulator\n");
        printf("  dump <file> Hex dump the ROM file\n");
        printf("  disassemble <file> Disassemble the ROM file\n");
        return 1;
    }

    const char *option = argv[1];

    if (!strcmp(option, "run")) {
        // Create and run emulator
        Emulator* emu = emulator_create();
        if (!emu) {
            fprintf(stderr, "Failed to create emulator\n");
            return 1;
        }

        emulator_run(emu);
        emulator_destroy(emu);
        return 0;
    } else if (argc >= 3) {
        const char *filename = argv[2];

        if (!strcmp(option, "dump")) {
            return hexdump(filename);
        } else if (!strcmp(option, "disassemble")) {
            disassembleROM(filename);
            return 0;
        }
    }

    printf("Unknown option or missing filename: %s\n", option);
    return 1;
}
