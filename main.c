#include <stdio.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
#include "deprecated/stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"


int main(int argc, char const *argv[]) {
    /* code */
    const char *origin = "test.jpg";
    int width, height, channels;
    unsigned char* image = stbi_load(origin, &width, &height, &channels, 0);
    if (image == NULL) {
        printf("Error in loading the image !\n");
        return -1;
    }

    int output_width = width * 0.5;  // new width
    int output_height = height * 0.5;  // new height
    unsigned char* output_image = (unsigned char*) malloc(output_width * output_height * channels);
    if (output_image == NULL) {
        printf("Error in allocating memory for the output image !\n");
        return -1;
    }

    const char *output = "output.jpg";
    stbir_resize_uint8(image, width, height, 0, output_image, output_width, output_height, 0, channels);
    int ret = stbi_write_jpg(output, output_width, output_height, channels, output_image, 100);

    const char *save_name_png = "output.png";
    ret = stbi_write_png(save_name_png, output_width, output_height, channels, output_image, 0);
    if (ret == 0) {
        fprintf(stderr, "fail to write image png: %s\n", image);
        return -1;
    }

    stbi_image_free(image);
    free(output_image);
    
    return 0;
}



