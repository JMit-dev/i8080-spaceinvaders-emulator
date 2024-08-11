#include <stdio.h>
#include <stdlib.h>

#include "hexdump.h"

int hexdump(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    unsigned char buffer[16];
    size_t bytesRead;
    size_t offset = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        printf("%04zx  ", offset);

        for (size_t i = 0; i < bytesRead; i++) {
            printf("%02x ", buffer[i]);
        }

        for (size_t i = bytesRead; i < 16; i++) {
            printf("   ");
        }

        printf("\n");

        offset += bytesRead;
    }

    fclose(file);
    return 0;
}
