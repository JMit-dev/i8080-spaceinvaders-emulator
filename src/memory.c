#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

void initializeMemory(State8080 *state, uint32_t size) {
    state->memory = malloc(size);
    if (state->memory == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
}

void freeMemory(State8080* state) {
    free(state->memory);
}

void loadROM(State8080* state, const char* filename, uint32_t offset) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Error: Couldn't open %s\n", filename);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    uint8_t *buffer = &state->memory[offset];
    fread(buffer, fsize, 1, f);
    fclose(f);
}