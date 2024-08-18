#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "i8080.h"

void initializeMemory(State8080 *state, uint32_t size);
void freeMemory(State8080* state);
void loadROM(State8080* state, const char* filename, uint32_t offset);

#endif
