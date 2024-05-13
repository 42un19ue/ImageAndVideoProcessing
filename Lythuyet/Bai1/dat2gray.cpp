#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WIDTH 2048
#define HEIGHT 1365
#define FILE_RED "datfile/red_image.dat"
#define FILE_GREEN "datfile/green_image.dat"
#define FILE_BLUE "datfile/blue_image.dat"
#define FILE_GRAY "datfile/grayscale.dat"

typedef struct {
    uint8_t red, green, blue;
} pixel;

pixel read_pixel(FILE* f) {
    pixel p;
    p.red = fgetc(f);
    p.green = fgetc(f);
    p.blue = fgetc(f);
    return p;
}

uint8_t to_gray(pixel p) {
    return (uint8_t)(0.299 * p.red + 0.587 * p.green + 0.114 * p.blue);
}

int main() {
    FILE* fred = fopen(FILE_RED, "rb");
    FILE* fgreen = fopen(FILE_GREEN, "rb");
    FILE* fblue = fopen(FILE_BLUE, "rb");
    if (fred == NULL || fgreen == NULL || fblue == NULL) {
        printf("Error: cannot open input file.\n");
        return -1;
    }

    FILE* fout = fopen(FILE_GRAY, "wb");
    if (fout == NULL) {
        printf("Error: cannot open output file.\n");
        return -1;
    }

    pixel p;
    uint8_t gray;
    uint8_t *buffer = (uint8_t*)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            p.red = fgetc(fred);
            p.green = fgetc(fgreen);
            p.blue = fgetc(fblue);
            gray = to_gray(p);
            fputc(gray, fout);
            buffer[y * WIDTH + x] = gray;
        }
    }

    printf("Xử ý ảnh thành công\n");
    fclose(fred);
    fclose(fgreen);
    fclose(fblue);
    fclose(fout);
    free(buffer);
    return 0;
}
