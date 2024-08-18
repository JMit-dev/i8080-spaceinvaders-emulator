#include "io.h"
#include "SDL2/SDL.h"

void initializeIO() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

void shutdownIO() {
    SDL_Quit();
}

uint8_t readPort(uint8_t port) {
    switch (port) {
        case 1:
            return inputPort1;
        case 2:
            return inputPort2;
        case 3:
            return (shiftRegister >> (8 - shiftOffset)) & 0xFF;
        default:
            return 0x00;
    }
}

void writePort(uint8_t port, uint8_t value) {
    switch (port) {
        case 2:
            shiftOffset = value & 0x07;
            break;
        case 4:
            shiftRegister = (shiftRegister >> 8) | (value << 8);
            break;
        case 3:
        case 5:
        case 6:
            break;
        default:
            break;
    }
}

void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            uint8_t keyDown = (event.type == SDL_KEYDOWN) ? 1 : 0;

            switch (event.key.keysym.sym) {
                // inputPort1 keys
                case SDLK_c:  // coin inserted
                    if (keyDown) inputPort1 |= 0x01;
                    else inputPort1 &= ~0x01;
                    break;
                case SDLK_2:  // P2 start button
                    if (keyDown) inputPort1 |= 0x02;
                    else inputPort1 &= ~0x02;
                    break;
                case SDLK_1:  // P1 start button
                    if (keyDown) inputPort1 |= 0x04;
                    else inputPort1 &= ~0x04;
                    break;
                case SDLK_SPACE:  // P1 shoot button
                    if (keyDown) inputPort1 |= 0x10;
                    else inputPort1 &= ~0x10;
                    break;
                case SDLK_LEFT:  // P1 joystick left
                    if (keyDown) inputPort1 |= 0x20;
                    else inputPort1 &= ~0x20;
                    break;
                case SDLK_RIGHT:  // P1 joystick right
                    if (keyDown) inputPort1 |= 0x40;
                    else inputPort1 &= ~0x40;
                    break;

                // inputPort2 keys
                case SDLK_t:  // tilt button
                    if (keyDown) inputPort2 |= 0x04;
                    else inputPort2 &= ~0x04;
                    break;
                case SDLK_b:  // bonus life dipswitch
                    if (keyDown) inputPort2 |= 0x08;
                    else inputPort2 &= ~0x08;
                    break;
                case SDLK_x:  // P2 shoot button
                    if (keyDown) inputPort2 |= 0x10;
                    else inputPort2 &= ~0x10;
                    break;
                case SDLK_a:  // P2 joystick left
                    if (keyDown) inputPort2 |= 0x20;
                    else inputPort2 &= ~0x20;
                    break;
                case SDLK_d:  // P2 joystick right
                    if (keyDown) inputPort2 |= 0x40;
                    else inputPort2 &= ~0x40;
                    break;
                case SDLK_v:  // coin info dipswitch
                    if (keyDown) inputPort2 |= 0x80;
                    else inputPort2 &= ~0x80;
                    break;
            }
        }
    }
}