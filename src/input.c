#include "input.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

struct Input {
    uint8_t port1;  // Player 1 controls and coin
    uint8_t port2;  // Player 2 controls and dip switches
};

Input* input_create(void) {
    Input* input = malloc(sizeof(Input));
    if (!input) return NULL;

    input->port1 = 0x00;
    input->port2 = 0x00;

    return input;
}

void input_destroy(Input* input) {
    free(input);
}

void input_update(Input* input) {
    if (!input) return;

    const uint8_t* keyboard_state = SDL_GetKeyboardState(NULL);

    // Update Port 1
    input->port1 = 0x00;
    if (keyboard_state[SDL_SCANCODE_C])     input->port1 |= 0x01;  // Coin
    if (keyboard_state[SDL_SCANCODE_2])     input->port1 |= 0x02;  // P2 Start
    if (keyboard_state[SDL_SCANCODE_1])     input->port1 |= 0x04;  // P1 Start
    if (keyboard_state[SDL_SCANCODE_SPACE]) input->port1 |= 0x10;  // P1 Shoot
    if (keyboard_state[SDL_SCANCODE_LEFT])  input->port1 |= 0x20;  // P1 Left
    if (keyboard_state[SDL_SCANCODE_RIGHT]) input->port1 |= 0x40;  // P1 Right

    // Update Port 2
    input->port2 = 0x00;
    if (keyboard_state[SDL_SCANCODE_T])     input->port2 |= 0x04;  // Tilt
    if (keyboard_state[SDL_SCANCODE_B])     input->port2 |= 0x08;  // Bonus life dipswitch
    if (keyboard_state[SDL_SCANCODE_X])     input->port2 |= 0x10;  // P2 Shoot
    if (keyboard_state[SDL_SCANCODE_A])     input->port2 |= 0x20;  // P2 Left
    if (keyboard_state[SDL_SCANCODE_D])     input->port2 |= 0x40;  // P2 Right
    if (keyboard_state[SDL_SCANCODE_V])     input->port2 |= 0x80;  // Coin info dipswitch
}

uint8_t input_get_port1(Input* input) {
    return input ? input->port1 : 0x00;
}

uint8_t input_get_port2(Input* input) {
    return input ? input->port2 : 0x00;
}
