#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "emulator.h"
#include "memory.h"

// Forward declare port handlers
static uint8_t port_read_handler(uint8_t port);
static void port_write_handler(uint8_t port, uint8_t value);

// Global IO state for port callbacks
static IOState* g_io_state = NULL;

Emulator* emulator_create(void) {
    Emulator* emu = malloc(sizeof(Emulator));
    if (!emu) return NULL;

    // Create CPU
    emu->cpu = calloc(1, sizeof(State8080));
    if (!emu->cpu) {
        free(emu);
        return NULL;
    }

    // Load ROM
    initializeMemory(emu->cpu, 0x10000);
    loadROM(emu->cpu, "./roms/invaders.h", 0x0000);
    loadROM(emu->cpu, "./roms/invaders.g", 0x0800);
    loadROM(emu->cpu, "./roms/invaders.f", 0x1000);
    loadROM(emu->cpu, "./roms/invaders.e", 0x1800);

    // Create platform (SDL2 window and rendering)
    emu->platform = platform_create("Space Invaders", 224, 256);
    if (!emu->platform) {
        freeMemory(emu->cpu);
        free(emu->cpu);
        free(emu);
        return NULL;
    }

    // Create input handler
    emu->input = input_create();
    if (!emu->input) {
        platform_destroy(emu->platform);
        freeMemory(emu->cpu);
        free(emu->cpu);
        free(emu);
        return NULL;
    }

    // Create I/O handler
    emu->io = io_create(emu->input);
    if (!emu->io) {
        input_destroy(emu->input);
        platform_destroy(emu->platform);
        freeMemory(emu->cpu);
        free(emu->cpu);
        free(emu);
        return NULL;
    }

    // Set global IO state for port callbacks
    g_io_state = emu->io;

    return emu;
}

void emulator_destroy(Emulator* emu) {
    if (!emu) return;

    g_io_state = NULL;

    if (emu->io) io_destroy(emu->io);
    if (emu->input) input_destroy(emu->input);
    if (emu->platform) platform_destroy(emu->platform);
    if (emu->cpu) {
        freeMemory(emu->cpu);
        free(emu->cpu);
    }

    free(emu);
}

void emulator_run(Emulator* emu) {
    if (!emu) return;

    int which_interrupt = 1;  // Alternate between RST 1 and RST 2

    // Space Invaders runs at 2MHz
    // At 60 FPS: 2000000 / 60 = 33333 cycles per frame
    // Two interrupts per frame: 33333 / 2 = 16666 cycles between interrupts
    const uint64_t CYCLES_PER_INTERRUPT = 16666;
    uint64_t next_interrupt_cycles = CYCLES_PER_INTERRUPT;

    bool running = true;
    while (running) {
        // Process platform events (quit, etc.)
        running = platform_process_events(emu->platform);

        // Update input state
        input_update(emu->input);

        // Execute one CPU instruction
        emulate8080(emu->cpu, port_read_handler, port_write_handler);

        // Check if it's time for an interrupt
        if (emu->cpu->cycles >= next_interrupt_cycles) {
            if (emu->cpu->int_enable) {
                GenerateInterrupt(emu->cpu, which_interrupt);

                // Only render on RST 2 (end of frame / VBLANK)
                if (which_interrupt == 2) {
                    platform_render_frame(emu->platform, emu->cpu->memory + 0x2400);
                }

                // Alternate interrupts
                which_interrupt = (which_interrupt == 1) ? 2 : 1;
                next_interrupt_cycles += CYCLES_PER_INTERRUPT;
            }
        }
    }
}

// Port callback handlers (bridge to global IO state)
static uint8_t port_read_handler(uint8_t port) {
    return io_read_port(g_io_state, port);
}

static void port_write_handler(uint8_t port, uint8_t value) {
    io_write_port(g_io_state, port, value);
}
