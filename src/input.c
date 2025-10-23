#include "input.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    SDL_Scancode p1_left;
    SDL_Scancode p1_right;
    SDL_Scancode p1_shoot;
    SDL_Scancode p1_start;
    SDL_Scancode p2_left;
    SDL_Scancode p2_right;
    SDL_Scancode p2_shoot;
    SDL_Scancode p2_start;
    SDL_Scancode coin;
    SDL_Scancode tilt;
    SDL_Scancode bonus_life;
    SDL_Scancode coin_info;
} InputConfig;

struct Input {
    uint8_t port1;  // Player 1 controls and coin
    uint8_t port2;  // Player 2 controls and dip switches
    InputConfig config;
};

static void set_default_config(InputConfig* config) {
    config->p1_left = SDL_SCANCODE_LEFT;
    config->p1_right = SDL_SCANCODE_RIGHT;
    config->p1_shoot = SDL_SCANCODE_SPACE;
    config->p1_start = SDL_SCANCODE_1;
    config->p2_left = SDL_SCANCODE_A;
    config->p2_right = SDL_SCANCODE_D;
    config->p2_shoot = SDL_SCANCODE_X;
    config->p2_start = SDL_SCANCODE_2;
    config->coin = SDL_SCANCODE_C;
    config->tilt = SDL_SCANCODE_T;
    config->bonus_life = SDL_SCANCODE_B;
    config->coin_info = SDL_SCANCODE_V;
}

static void trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

static SDL_Scancode parse_scancode(const char* name) {
    return SDL_GetScancodeFromName(name);
}

static bool load_config(const char* filename, InputConfig* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;

        char* equals = strchr(line, '=');
        if (!equals) continue;

        *equals = '\0';
        char* key = line;
        char* value = equals + 1;

        trim(key);
        trim(value);

        SDL_Scancode scancode = parse_scancode(value);
        if (scancode == SDL_SCANCODE_UNKNOWN) continue;

        if (strcmp(key, "P1_LEFT") == 0) config->p1_left = scancode;
        else if (strcmp(key, "P1_RIGHT") == 0) config->p1_right = scancode;
        else if (strcmp(key, "P1_SHOOT") == 0) config->p1_shoot = scancode;
        else if (strcmp(key, "P1_START") == 0) config->p1_start = scancode;
        else if (strcmp(key, "P2_LEFT") == 0) config->p2_left = scancode;
        else if (strcmp(key, "P2_RIGHT") == 0) config->p2_right = scancode;
        else if (strcmp(key, "P2_SHOOT") == 0) config->p2_shoot = scancode;
        else if (strcmp(key, "P2_START") == 0) config->p2_start = scancode;
        else if (strcmp(key, "COIN") == 0) config->coin = scancode;
        else if (strcmp(key, "TILT") == 0) config->tilt = scancode;
        else if (strcmp(key, "BONUS_LIFE") == 0) config->bonus_life = scancode;
        else if (strcmp(key, "COIN_INFO") == 0) config->coin_info = scancode;
    }

    fclose(file);
    return true;
}

Input* input_create(void) {
    Input* input = malloc(sizeof(Input));
    if (!input) return NULL;

    input->port1 = 0x00;
    input->port2 = 0x00;
    set_default_config(&input->config);

    return input;
}

Input* input_create_from_config(const char* config_file) {
    Input* input = input_create();
    if (!input) return NULL;

    if (!load_config(config_file, &input->config)) {
        fprintf(stderr, "Warning: Could not load config file '%s', using defaults\n", config_file);
    }

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
    if (keyboard_state[input->config.coin])     input->port1 |= 0x01;  // Coin
    if (keyboard_state[input->config.p2_start]) input->port1 |= 0x02;  // P2 Start
    if (keyboard_state[input->config.p1_start]) input->port1 |= 0x04;  // P1 Start
    if (keyboard_state[input->config.p1_shoot]) input->port1 |= 0x10;  // P1 Shoot
    if (keyboard_state[input->config.p1_left])  input->port1 |= 0x20;  // P1 Left
    if (keyboard_state[input->config.p1_right]) input->port1 |= 0x40;  // P1 Right

    // Update Port 2
    input->port2 = 0x00;
    if (keyboard_state[input->config.tilt])       input->port2 |= 0x04;  // Tilt
    if (keyboard_state[input->config.bonus_life]) input->port2 |= 0x08;  // Bonus life dipswitch
    if (keyboard_state[input->config.p2_shoot])   input->port2 |= 0x10;  // P2 Shoot
    if (keyboard_state[input->config.p2_left])    input->port2 |= 0x20;  // P2 Left
    if (keyboard_state[input->config.p2_right])   input->port2 |= 0x40;  // P2 Right
    if (keyboard_state[input->config.coin_info])  input->port2 |= 0x80;  // Coin info dipswitch
}

uint8_t input_get_port1(Input* input) {
    return input ? input->port1 : 0x00;
}

uint8_t input_get_port2(Input* input) {
    return input ? input->port2 : 0x00;
}
