#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

// Input abstraction layer - maps platform input to emulator input ports
// Separates SDL keyboard handling from Space Invaders I/O port logic

typedef struct Input Input;

// Input lifecycle
Input* input_create(void);
void input_destroy(Input* input);

// Update input state from platform events
void input_update(Input* input);

// Get input port values (Space Invaders specific)
uint8_t input_get_port1(Input* input);
uint8_t input_get_port2(Input* input);

#endif
