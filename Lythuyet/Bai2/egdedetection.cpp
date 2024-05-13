#include <stdio.h>
#include <math.h>

#define IMAGE_HEIGHT 320
#define IMAGE_WIDTH 320
#define KERNEL_HEIGHT 3
#define KERNEL_WIDTH 3

void readFile(const char* inputName, unsigned char gray[][IMAGE_WIDTH]) {
    FILE* f = fopen(inputName, "rb");
    fread(gray, sizeof(unsigned char), IMAGE_WIDTH * IMAGE_HEIGHT, f);
    fclose(f);
}

void writeFile(const char* outputName, unsigned char edgedetec[][IMAGE_WIDTH]) {
    FILE* f = fopen(outputName, "wb");
    fwrite(edgedetec, sizeof(unsigned char), IMAGE_WIDTH * IMAGE_HEIGHT, f);
    printf("Thanh cong");
    fclose(f);
}

void tichchap(unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH], int kernel[KERNEL_HEIGHT][KERNEL_WIDTH], unsigned char result[IMAGE_HEIGHT][IMAGE_WIDTH]) {
    int kCenterX = KERNEL_HEIGHT / 2;
    int kCenterY = KERNEL_WIDTH / 2;

    for (int i = 0; i < IMAGE_HEIGHT; ++i) {
        for (int j = 0; j < IMAGE_WIDTH; ++j) {
            long int sum = 0;

            for (int k = 0; k < KERNEL_HEIGHT; ++k) {
                int kk = KERNEL_HEIGHT - 1 - k;
                for (int n = 0; n < KERNEL_WIDTH; ++n) {
                    int nn = KERNEL_WIDTH - 1 - n;
                    int ii = i + (kCenterY - kk);
                    int jj = j + (kCenterX - nn);

                    if (ii >= 0 && ii < IMAGE_HEIGHT && jj >= 0 && jj < IMAGE_WIDTH) {
                        sum += image[ii][jj] * kernel[kk][nn];
                    }
                }
            }

            result[i][j] = (unsigned char)sum;
        }
    }
}

void G(unsigned char Gx[IMAGE_HEIGHT][IMAGE_WIDTH], unsigned char Gy[IMAGE_HEIGHT][IMAGE_WIDTH], unsigned char result[IMAGE_HEIGHT][IMAGE_WIDTH]) {
    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
            result[i][j] = sqrt((Gx[i][j]^2 + Gy[i][j]^2));
        }
    }
}



int main() {
    unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH];
    unsigned char result[IMAGE_HEIGHT][IMAGE_WIDTH];
    unsigned char result1[IMAGE_HEIGHT][IMAGE_WIDTH];
    unsigned char result2[IMAGE_HEIGHT][IMAGE_WIDTH]; 

    int kernel_x[KERNEL_HEIGHT][KERNEL_WIDTH] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int kernel_y[KERNEL_HEIGHT][KERNEL_WIDTH] = {
        {-1, -2, 1},
        {0, 0, 0},
        {1, 2, 1}
    };

    const char* inputName = "datfile/raw.dat";
    const char* outputName = "datfile/edge.dat";

    readFile(inputName, image);
    tichchap(image, kernel_x, result1);
    tichchap(image, kernel_y, result2);
    G(result1, result2, result);

    writeFile(outputName, result);

    return 0;
}
