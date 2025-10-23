#include <string.h>

#include "display.h"

void initializeDisplay(Display *display, int width, int height) {
    display->width = width;
    display->height = height;
    display->window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(display->window, width, height);
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(display->renderer, width, height);
    display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    display->pixels = malloc(width * height * sizeof(uint32_t));
}

void drawDisplay(Display *display, uint8_t *videoMemory) {
    memset(display->pixels, 0, display->width * display->height * sizeof(uint32_t));

    for (int x = 0; x < display->width; x++) {
        for (int y = 0; y < display->height / 8; y++) {
            uint8_t byte = videoMemory[y + x * (display->height / 8)];
            for (int bit = 0; bit < 8; bit++) {
                int pixelY = display->height - 1 - (y * 8 + bit);
                int pixelX = x;

                display->pixels[pixelY * display->width + pixelX] = (byte & (1 << bit)) ? 0xFFFFFFFF : 0xFF000000;
            }
        }
    }

    SDL_UpdateTexture(display->texture, NULL, display->pixels, display->width * sizeof(uint32_t));

    SDL_RenderClear(display->renderer);
    SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
    SDL_RenderPresent(display->renderer);
}

void destroyDisplay(Display *display) {
    free(display->pixels);
    SDL_DestroyTexture(display->texture);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
}
