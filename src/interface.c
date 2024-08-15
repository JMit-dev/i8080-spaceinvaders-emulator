#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i8080.h"
#include "hexdump.h"
#include "disassembler.h"

void loadROM(State8080* state, const char* filename, uint32_t offset)
{
	FILE *f= fopen(filename, "rb");
	if (f==NULL) {
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}

State8080* initializeState() {
    State8080 *state = calloc(1, sizeof(State8080));
    state->memory = malloc(0x10000);  // 16kb
    return state;
}

void runEmulator(State8080 *state) {
    while (1) {
        emulate8080Op(state);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <ROM file>\n", argv[0]);
        printf("Options:\n");
        printf("  dump        Hex dump the ROM file\n");
        printf("  disassemble Disassemble the ROM file\n");
        printf("  run         Run the ROM on the CPU\n");
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
        State8080 *state = initializeState();
        loadROM(state, filename, 0);
        // loadROM(state, "./invaders/invaders.h", 0);
	    // loadROM(state, "./invaders/invaders.g", 0x800);
	    // loadROM(state, "./invaders/invaders.f", 0x1000);
	    // loadROM(state, "./invaders/invaders.e", 0x1800);
        runEmulator(state);
        free(state->memory);
        free(state);
        return 0;
    } else {
        printf("Unknown option: %s\n", option);
        return 1;
    }
}
