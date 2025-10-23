#include "platform.h"
#include "display.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

struct Platform {
    Display* display;
};

Platform* platform_create(const char* title, int width, int height) {
    Platform* platform = malloc(sizeof(Platform));
    if (!platform) return NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        free(platform);
        return NULL;
    }

    // Create display
    platform->display = malloc(sizeof(Display));
    if (!platform->display) {
        SDL_Quit();
        free(platform);
        return NULL;
    }

    initializeDisplay(platform->display, width, height);
    return platform;
}

void platform_destroy(Platform* platform) {
    if (!platform) return;

    if (platform->display) {
        destroyDisplay(platform->display);
        free(platform->display);
    }

    SDL_Quit();
    free(platform);
}

bool platform_process_events(Platform* platform) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void platform_render_frame(Platform* platform, const uint8_t* video_memory) {
    if (platform && platform->display) {
        drawDisplay(platform->display, (uint8_t*)video_memory);
    }
}

uint32_t platform_get_ticks(void) {
    return SDL_GetTicks();
}

void platform_delay(uint32_t ms) {
    SDL_Delay(ms);
}
