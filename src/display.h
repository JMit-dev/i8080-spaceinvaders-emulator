#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct Display {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t *pixels;
    int width;
    int height;
} Display;

void initializeDisplay(Display*, int, int);
void drawDisplay(Display*, uint8_t*);
void destroyDisplay(Display*);

#endif
