#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "i8080.h"

void initializeMemory(State8080*, uint32_t);
void freeMemory(State8080*);
void loadROM(State8080*, const char*, uint32_t);

#endif
