#include <stdio.h>
#include <stdlib.h>
#include "turbojpeg.h"

/**
 * https://github.com/libjpeg-turbo/libjpeg-turbo
 */

int resizeJPEG(const char *inputFilename, const char *outputFilename, int scaleFactor) {
    tjhandle decompressor = NULL;
    unsigned char *inputImage = NULL, *outputImage = NULL;
    unsigned long inputSize = 0, outputSize = 0;
    int width, height, jpegSubsamp, jpegColorspace;
    int scaledWidth, scaledHeight;
    int flags = 0; // Normal operation
    long size;

    FILE *inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        fprintf(stderr, "Cannot open input file %s\n", inputFilename);
        return -1;
    }

    // 获取文件大小
    fseek(inputFile, 0, SEEK_END);
    size = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // 分配内存并读取图片
    inputImage = (unsigned char *) malloc(size);
    if (fread(inputImage, 1, size, inputFile) < size) {
        fprintf(stderr, "Cannot read input file\n");
        fclose(inputFile);
        free(inputImage);
        return -1;
    }
    fclose(inputFile);

    // 初始化TurboJPEG解压缩句柄
    decompressor = tjInitDecompress();
    // 获取图片参数
    tjDecompressHeader3(decompressor, inputImage, size, &width,
                        &height, &jpegSubsamp, &jpegColorspace);

    // 计算新尺寸
    scaledWidth = width / scaleFactor;
    scaledHeight = height / scaleFactor;

    // 分配输出图片内存
    outputSize = tjBufSize(scaledWidth, scaledHeight, jpegSubsamp);
    outputImage = (unsigned char *) malloc(outputSize);

    // 执行缩放
    if (tjDecompress2(decompressor, inputImage, size, outputImage,
                      scaledWidth, 0, scaledHeight, TJPF_RGB, flags) < 0) {
        fprintf(stderr, "Decompression error: %s\n", tjGetErrorStr());
        tjDestroy(decompressor);
        free(inputImage);
        free(outputImage);
        return -1;
    }

    // 保存缩放后的图片
    tjhandle compressor = tjInitCompress();
    if (tjCompress2(compressor, outputImage, scaledWidth, 0, scaledHeight,
                    TJPF_RGB, &inputImage, &inputSize, jpegSubsamp, 80, flags) < 0) {
        fprintf(stderr, "Compression error: %s\n", tjGetErrorStr());
        tjDestroy(decompressor);
        tjDestroy(compressor);
        free(inputImage);
        free(outputImage);
        return -1;
    }

    FILE *outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        fprintf(stderr, "Cannot open output file %s\n", outputFilename);
        tjDestroy(decompressor);
        tjDestroy(compressor);
        free(inputImage);
        free(outputImage);
        return -1;
    }

    if (fwrite(inputImage, inputSize, 1, outputFile) < 1) {
        fprintf(stderr, "Cannot write to output file\n");
        fclose(outputFile);
        tjDestroy(decompressor);
        tjDestroy(compressor);
        free(inputImage);
        free(outputImage);
        return -1;
    }
    fclose(outputFile);

    // 清理资源
    tjDestroy(decompressor);
    tjDestroy(compressor);
    free(inputImage);
    free(outputImage);
    return 0;
}

int main(int argc, char const *argv[]) {
    /* code */
    char *input_filename = "/data/user/0/com.cyrabbit.momarsapp/files/8kr5xwDcpumfDe7/libc.jpeg";
    char *output_filename = "/data/user/0/com.cyrabbit.momarsapp/files/8kr5xwDcpumfDe7/output.jpeg";
    if (resizeJPEG(input_filename, output_filename, 2) == 0) {
        printf("Image resized successfully.\n");
    } else {
        printf("Failed to resize image.\n");
    }

    return 0;
}
