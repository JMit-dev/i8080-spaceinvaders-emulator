#include <stdio.h>
#include <stdlib.h>

#include "hexdump.h"

static void hexdump_file(const char *filename, FILE* output) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    unsigned char buffer[16];
    size_t bytesRead;
    size_t offset = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fprintf(output, "%04zx  ", offset);

        for (size_t i = 0; i < bytesRead; i++) {
            fprintf(output, "%02x ", buffer[i]);
        }

        for (size_t i = bytesRead; i < 16; i++) {
            fprintf(output, "   ");
        }

        fprintf(output, "\n");
        offset += bytesRead;
    }

    fclose(file);
}

void hexdump_space_invaders(FILE* output) {
    fprintf(output, "=== Space Invaders ROM Hex Dump ===\n\n");
    fprintf(output, "--- invaders.h (0x0000-0x07FF) ---\n");
    hexdump_file("./roms/invaders.h", output);
    fprintf(output, "\n--- invaders.g (0x0800-0x0FFF) ---\n");
    hexdump_file("./roms/invaders.g", output);
    fprintf(output, "\n--- invaders.f (0x1000-0x17FF) ---\n");
    hexdump_file("./roms/invaders.f", output);
    fprintf(output, "\n--- invaders.e (0x1800-0x1FFF) ---\n");
    hexdump_file("./roms/invaders.e", output);
}
