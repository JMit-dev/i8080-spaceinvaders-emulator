#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hexdump.h"
#include "disassembler.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <ROM file>\n", argv[0]);
        printf("Options:\n");
        printf("  dump        Hex dump the ROM file\n");
        printf("  disassemble Disassemble the ROM file\n");
        printf("  run         Run the ROM on the CPU (not yet implemented)\n");
        return 1;
    }

    const char *option = argv[1];
    const char *filename = argv[2];

    if (strcmp(option, "dump") == 0) {
        return hexdump(filename);
    } else if (strcmp(option, "disassemble") == 0) {
        disassembleROM(filename);
        return 0;
    } else if (strcmp(option, "run") == 0) {
        printf("Run option is not yet implemented.\n");
        return 1;
    } else {
        printf("Unknown option: %s\n", option);
        return 1;
    }
}
