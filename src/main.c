#include <stdio.h>
#include <string.h>

#define SDL_MAIN_HANDLED

#include "hexdump.h"
#include "disassembler.h"
#include "emulator.h"
#include "memory.h"
#include "i8080.h"

static void print_usage(const char* prog_name) {
    printf("Intel 8080 Space Invaders Emulator\n\n");
    printf("Usage: %s <command> [options]\n\n", prog_name);
    printf("Commands:\n");
    printf("  run                    Run Space Invaders\n");
    printf("  test                   Run CPU diagnostic test (cpudiag.bin)\n");
    printf("  dump [output_file]     Hex dump Space Invaders ROM\n");
    printf("  disasm [output_file]   Disassemble Space Invaders ROM\n");
    printf("\n");
    printf("Options:\n");
    printf("  If output_file is not specified, output goes to stdout\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s run\n", prog_name);
    printf("  %s test\n", prog_name);
    printf("  %s dump invaders_dump.txt\n", prog_name);
    printf("  %s disasm invaders_disasm.txt\n", prog_name);
}

int main(int argc, char *argv[]) {
    // Default to "run" if no arguments (allows double-click to launch)
    const char *command = (argc < 2) ? "run" : argv[1];

    if (!strcmp(command, "run")) {
        Emulator* emu = emulator_create(ROM_SPACE_INVADERS);
        if (!emu) {
            fprintf(stderr, "Failed to create emulator\n");
            return 1;
        }
        emulator_run(emu);
        emulator_destroy(emu);
        return 0;
    }
    else if (!strcmp(command, "test")) {
        Emulator* emu = emulator_create(ROM_CPU_DIAG);
        if (!emu) {
            fprintf(stderr, "Failed to create emulator\n");
            return 1;
        }
        emulator_run_headless(emu);
        emulator_destroy(emu);
        return 0;
    }
    else if (!strcmp(command, "dump")) {
        FILE* output = stdout;
        if (argc >= 3) {
            output = fopen(argv[2], "w");
            if (!output) {
                fprintf(stderr, "Failed to open output file: %s\n", argv[2]);
                return 1;
            }
        }
        hexdump_space_invaders(output);
        if (output != stdout) {
            fclose(output);
            printf("Hex dump written to %s\n", argv[2]);
        }
        return 0;
    }
    else if (!strcmp(command, "disasm")) {
        FILE* output = stdout;
        if (argc >= 3) {
            output = fopen(argv[2], "w");
            if (!output) {
                fprintf(stderr, "Failed to open output file: %s\n", argv[2]);
                return 1;
            }
        }
        disassemble_space_invaders(output);
        if (output != stdout) {
            fclose(output);
            printf("Disassembly written to %s\n", argv[2]);
        }
        return 0;
    }
    else {
        fprintf(stderr, "Unknown command: %s\n\n", command);
        print_usage(argv[0]);
        return 1;
    }
}
