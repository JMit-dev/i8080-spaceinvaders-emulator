#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stdbool.h>

// Platform abstraction layer - handles all SDL2/OS-specific code
// This allows the emulator core to remain platform-agnostic

typedef struct Platform Platform;

// Platform lifecycle
Platform* platform_create(const char* title, int width, int height);
void platform_destroy(Platform* platform);

// Event handling
bool platform_process_events(Platform* platform);  // Returns false when quit requested

// Rendering
void platform_render_frame(Platform* platform, const uint8_t* video_memory);

// Timing
uint32_t platform_get_ticks(void);
void platform_delay(uint32_t ms);

#endif
