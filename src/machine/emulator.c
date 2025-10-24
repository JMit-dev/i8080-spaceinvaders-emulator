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

Emulator* emulator_create(ROMType rom_type) {
    Emulator* emu = malloc(sizeof(Emulator));
    if (!emu) return NULL;

    // Create CPU
    emu->cpu = calloc(1, sizeof(State8080));
    if (!emu->cpu) {
        free(emu);
        return NULL;
    }

    // Load ROM based on type
    initializeMemory(emu->cpu, 0x10000);

    switch (rom_type) {
        case ROM_SPACE_INVADERS:
            // Try current directory first (for releases), then rsrc (for development)
            if (!loadROM(emu->cpu, "invaders.h", 0x0000)) {
                loadROM(emu->cpu, "./rsrc/invaders.h", 0x0000);
            }
            if (!loadROM(emu->cpu, "invaders.g", 0x0800)) {
                loadROM(emu->cpu, "./rsrc/invaders.g", 0x0800);
            }
            if (!loadROM(emu->cpu, "invaders.f", 0x1000)) {
                loadROM(emu->cpu, "./rsrc/invaders.f", 0x1000);
            }
            if (!loadROM(emu->cpu, "invaders.e", 0x1800)) {
                loadROM(emu->cpu, "./rsrc/invaders.e", 0x1800);
            }
            break;
        case ROM_CPU_DIAG:
            if (!loadROM(emu->cpu, "cpudiag.bin", 0x0100)) {
                loadROM(emu->cpu, "./rsrc/cpudiag.bin", 0x0100);
            }
            // Fix for CP/M BDOS call simulation
            emu->cpu->memory[0x0000] = 0xC3;  // JMP 0x0100
            emu->cpu->memory[0x0001] = 0x00;
            emu->cpu->memory[0x0002] = 0x01;
            emu->cpu->memory[0x0005] = 0xC9;  // RET (for BDOS calls)

            // Fix the stack pointer from 0x06ad to 0x07ad
            // LXI SP instruction is at offset 0xA9, so memory address is 0x100 + 0xA9 = 0x1A9
            // The high byte of the address is at 0x1AA
            emu->cpu->memory[0x1AA] = 0x7;

            emu->cpu->pc = 0x0100;
            break;
    }

    // Only create platform/input/io for Space Invaders (not for headless test mode)
    if (rom_type == ROM_SPACE_INVADERS) {
        // Create platform (SDL2 window and rendering)
        emu->platform = platform_create("Space Invaders", 224, 256);
        if (!emu->platform) {
            freeMemory(emu->cpu);
            free(emu->cpu);
            free(emu);
            return NULL;
        }

        // Create input handler (try to load from config file, fall back to defaults)
        emu->input = input_create_from_config("input.cfg");
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
    } else {
        // For CPU diagnostic test, no platform/input/io needed
        emu->platform = NULL;
        emu->input = NULL;
        emu->io = NULL;
    }

    // Set global IO state for port callbacks (only for Space Invaders)
    if (rom_type == ROM_SPACE_INVADERS) {
        g_io_state = emu->io;
    }

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
    const uint32_t MS_PER_FRAME = 16;  // 1000ms / 60fps = 16.67ms per frame

    uint64_t next_interrupt_cycles = CYCLES_PER_INTERRUPT;
    uint32_t next_frame_time = platform_get_ticks() + MS_PER_FRAME;

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

                    // Frame rate limiting: wait until it's time for next frame
                    uint32_t current_time = platform_get_ticks();
                    if (current_time < next_frame_time) {
                        platform_delay(next_frame_time - current_time);
                    }
                    next_frame_time = platform_get_ticks() + MS_PER_FRAME;
                }

                // Alternate interrupts
                which_interrupt = (which_interrupt == 1) ? 2 : 1;
                next_interrupt_cycles += CYCLES_PER_INTERRUPT;
            }
        }
    }
}

// Dummy port handlers for headless mode
static uint8_t headless_port_read(uint8_t port) {
    return 0;
}

static void headless_port_write(uint8_t port, uint8_t value) {
    // Not used by cpudiag
}

void emulator_run_headless(Emulator* emu) {
    if (!emu) return;

    printf("Running CPU diagnostic test...\n\n");

    // Run until HLT instruction
    while (emu->cpu->memory[emu->cpu->pc] != 0x76) {
        emulate8080(emu->cpu, headless_port_read, headless_port_write);
    }

    printf("\n\nCPU halted at PC=0x%04x\n", emu->cpu->pc);
}

// Port callback handlers (bridge to global IO state)
static uint8_t port_read_handler(uint8_t port) {
    return io_read_port(g_io_state, port);
}

static void port_write_handler(uint8_t port, uint8_t value) {
    io_write_port(g_io_state, port, value);
}
