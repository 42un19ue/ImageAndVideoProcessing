#include <stdio.h>
#include <math.h>
#include <string.h>

#define KERNEL_X 5
#define KERNEL_Y 5
#define WIDTH 256
#define HEIGHT 256

void readInput(const char *inputName, unsigned char in[], int width, int height)
{
    FILE *fptr = fopen(inputName, "rb");
    fread(in, sizeof(char), width * height, fptr);
    fclose(fptr);
}

void convertBinary(unsigned char bin_matrix[], unsigned char gray_matrix[], unsigned char threshold, int width, int height)
{
    unsigned char temp = 0x00;
    unsigned char countBit = 0;
    unsigned int index = 0;
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            if (countBit == 8)
            {
                bin_matrix[index++] = temp;
                temp = 0x00;
                countBit = 0;
            }
            temp <<= 1;
            countBit++;
            if (gray_matrix[x * HEIGHT + y] > threshold)
            {
                temp |= 0x01;
            }
        }
    }
}

void writeFileBin(const char *outputName, unsigned char bin_matrix[], int width, int height)
{
    FILE *fptr = fopen(outputName, "wb");
    fwrite(bin_matrix, sizeof(char), (width * height) / 8, fptr);
    fclose(fptr);
}

void conv2D(unsigned char in[], unsigned char out[], int width, int height,
            float kernel[], int kernelSizeX, int kernelSizeY)
{
    int i, j, m, n, mm, nn;
    int kCenterX, kCenterY;
    int rowIndex, colIndex;
    float sum;
    int index = 0;
    kCenterX = kernelSizeX / 2;
    kCenterY = kernelSizeY / 2;

    for (i = 0; i < height; ++i) 
    {
        for (j = 0; j < width; ++j) 
        {
            sum = 0;                        
            for (m = 0; m < kernelSizeY; ++m) 
            {
                mm = kernelSizeY - 1 - m;

                for (n = 0; n < kernelSizeX; ++n) 
                {
                    nn = kernelSizeX - 1 - n; 

                    rowIndex = i + m - kCenterY;
                    colIndex = j + n - kCenterX;

                    if (rowIndex >= 0 && rowIndex < height && colIndex >= 0 && colIndex < width)
                        sum += in[rowIndex * width + colIndex] * kernel[mm * kernelSizeX + nn];
                }
            }
            out[index++] = (unsigned char)fabs(sum);
        }
    }
}

unsigned char findMedian(unsigned char in_FH[], unsigned char in_FP[], unsigned char in_FV[], unsigned char in_FM[], int width, int height)
{

    unsigned char valueBuffer[256];
    for (int i = 0; i < 256; i++)
        valueBuffer[i] = 0;

    unsigned char index = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            index = in_FH[i * height + j];
            if (valueBuffer[index] == 0)
                valueBuffer[index] = 1;

            index = in_FP[i * height + j];
            if (valueBuffer[index] == 0)
                valueBuffer[index] = 1;

            index = in_FV[i * height + j];
            if (valueBuffer[index] == 0)
                valueBuffer[index] = 1;

            index = in_FM[i * height + j];
            if (valueBuffer[index] == 0)
                valueBuffer[index] = 1;
        }
    }

    int size = 0;
    unsigned char valueArr[256];
    for (int i = 0; i < 256; i++)
    {
        if (valueBuffer[i] == 1)
        {
            valueArr[size] = i;
            size += 1;
        }
    }

    unsigned char median = valueArr[size / 2];
    return median;
};

void winnerTakeAll(unsigned char in_FH[], unsigned char in_FP[], unsigned char in_FV[], unsigned char in_FM[], unsigned char medianValue, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char *maxPtr = &in_FH[i * height + j];
            if (*maxPtr < in_FP[i * height + j])
            {
                *maxPtr = 0;
                maxPtr = &in_FP[i * height + j];
            }
            else
            {
                in_FP[i * height + j] = 0;
            }

            if (*maxPtr < in_FV[i * height + j])
            {
                *maxPtr = 0;
                maxPtr = &in_FV[i * height + j];
            }
            else
            {
                in_FV[i * height + j] = 0;
            }

            if (*maxPtr < in_FM[i * height + j])
            {
                *maxPtr = 0;
                maxPtr = &in_FM[i * height + j];
            }
            else
            {
                in_FM[i * height + j] = 0;
            }

            if (*maxPtr > medianValue)
                *maxPtr = 1;
            else
                *maxPtr = 0;
        }
    }
}

void PPED(unsigned char in_FH[], unsigned char in_FP[], unsigned char in_FV[], unsigned char in_FM[], int PPED_vector[], int width, int height)
{
    unsigned char sizeBlockX = width / 4;
    unsigned char sizeBlockY = height / 4;
    int v1, v2, v3, v4;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            v1 = 0;
            v2 = 0;
            v3 = 0;
            v4 = 0;
            for (int m = i * sizeBlockY; m < (i + 1) * sizeBlockY; m++)
            {
                for (int n = j * sizeBlockX; n < (j + 1) * sizeBlockX; n++)
                {
                    v1 += in_FH[m * height + n];
                    v2 += in_FP[m * height + n];
                    v3 += in_FV[m * height + n];
                    v4 += in_FM[m * height + n];
                }
            }
            PPED_vector[i * 4 + j] = v1;
            printf("%d: ",PPED_vector[i * 4 + j]);
            PPED_vector[(i * 4 + j) + 16] = v2;
            printf("%d: ",PPED_vector[(i * 4 + j) + 16]);
            PPED_vector[(i * 4 + j) + 16 * 2] = v3;
            printf("%d: ",PPED_vector[(i * 4 + j) + 16 * 2]);
            PPED_vector[(i * 4 + j) + 16 * 3] = v4;
            printf("%d: ",PPED_vector[(i * 4 + j) + 16 * 3]);
        }
    }
}

void handlePPED(const char *inputPath, const char *outputPath, int PPED_vector[], int width, int height)
{
    float K_H[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0}; // Horizontal
    float K_P[] = {0, 0, 0, 1, 0, 0, 1, 1, 0, -1, 0, 1, 0, -1, 0, 1, 0, -1, -1, 0, 0, -1, 0, 0, 0}; // 45 Degree
    float K_V[] = {0, 1, 0, -1, 0, 0, 1, 0, -1, 0, 0, 1, 0, -1, 0, 0, 1, 0, -1, 0, 0, 1, 0, -1, 0}; // Vertical
    float K_M[] = {0, -1, 0, 0, 0, 1, 0, -1, -1, 0, 0, 1, 0, -1, 0, 0, 1, 1, 0, -1, 0, 0, 0, 1, 0}; // -45 Degree

    unsigned char person[WIDTH * HEIGHT];

    readInput(inputPath, person, WIDTH, HEIGHT);

    unsigned char person_FH[WIDTH * HEIGHT];
    unsigned char person_FP[WIDTH * HEIGHT];
    unsigned char person_FV[WIDTH * HEIGHT];
    unsigned char person_FM[WIDTH * HEIGHT];

    conv2D(person, person_FH, WIDTH, HEIGHT, K_H, KERNEL_X, KERNEL_Y);
    conv2D(person, person_FP, WIDTH, HEIGHT, K_P, KERNEL_X, KERNEL_Y);
    conv2D(person, person_FV, WIDTH, HEIGHT, K_V, KERNEL_X, KERNEL_Y);
    conv2D(person, person_FM, WIDTH, HEIGHT, K_M, KERNEL_X, KERNEL_Y);

    unsigned char medianValue = findMedian(person_FH, person_FP, person_FV, person_FM, WIDTH, HEIGHT);

    unsigned char bin_FH[(WIDTH * HEIGHT) / 8];
    unsigned char bin_FP[(WIDTH * HEIGHT) / 8];
    unsigned char bin_FV[(WIDTH * HEIGHT) / 8];
    unsigned char bin_FM[(WIDTH * HEIGHT) / 8];

    convertBinary(bin_FH, person_FH, medianValue, WIDTH, HEIGHT);
    convertBinary(bin_FP, person_FP, medianValue, WIDTH, HEIGHT);
    convertBinary(bin_FV, person_FV, medianValue, WIDTH, HEIGHT);
    convertBinary(bin_FM, person_FM, medianValue, WIDTH, HEIGHT);

    char outputNameFH[30];
    char outputNameFP[30];
    char outputNameFV[30];
    char outputNameFM[30];

    snprintf(outputNameFH, sizeof(outputNameFH), "%s/%s", outputPath, "bin_FH.dat");
    snprintf(outputNameFP, sizeof(outputNameFP), "%s/%s", outputPath, "bin_FP.dat");
    snprintf(outputNameFV, sizeof(outputNameFV), "%s/%s", outputPath, "bin_FV.dat");
    snprintf(outputNameFM, sizeof(outputNameFM), "%s/%s", outputPath, "bin_FM.dat");

    writeFileBin(outputNameFH, bin_FH, WIDTH, HEIGHT);
    writeFileBin(outputNameFP, bin_FP, WIDTH, HEIGHT);
    writeFileBin(outputNameFV, bin_FV, WIDTH, HEIGHT);
    writeFileBin(outputNameFM, bin_FM, WIDTH, HEIGHT);

    winnerTakeAll(person_FH, person_FP, person_FV, person_FM, medianValue, WIDTH, HEIGHT);

    PPED(person_FH, person_FP, person_FV, person_FM, PPED_vector, WIDTH, HEIGHT);

    printf("PPED vector:\n");
    printf("[");
    for (int i = 0; i < 64; i++)
    {
        printf("%d ", PPED_vector[i]);
    }
    printf("]\n\n");
}

float distance(int PPED_vector_1[], int PPED_vector_2[])
{
    float sum = 0;
    for (int i = 0; i < 64; i++)
    {
        sum += pow(PPED_vector_1[i] - PPED_vector_2[i], 2);
    }
    return sqrt(sum);
}

int main()
{
    const char *inputPerson_1 = "dat/musiala.dat";
    const char *inputPerson_2 = "dat/wirtz.dat";
    const char *input = "dat/who.dat";

    int PPED_vector_1[4 * 16];
    int PPED_vector_2[4 * 16];

    int whose_PPED_vector[4 * 16];

    handlePPED(inputPerson_1, "output/person1", PPED_vector_1, WIDTH, HEIGHT);
    handlePPED(inputPerson_2, "output/person2", PPED_vector_2, WIDTH, HEIGHT);
    handlePPED(input, "output/who", whose_PPED_vector, WIDTH, HEIGHT);

    float d1 = distance(PPED_vector_1, whose_PPED_vector);
    float d2 = distance(PPED_vector_2, whose_PPED_vector);

    printf("Distance between person 1 and who: %f\n", d1);
    printf("Distance between person 2 and who: %f\n", d2);

    if (d1 < d2)
        printf("Đây là Musiala\n");
    else
        printf("Đây là Wirtz\n");

    return 0;
}