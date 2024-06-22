#include <stdlib.h>
#include <stdio.h>
#include <png.h>

void process_file(char *input_path, char *output_path, int compression_level) {
    FILE *fp = fopen(input_path, "rb");
    if (!fp) {
        perror("File opening failed");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    fclose(fp);

    // Process for writing
    fp = fopen(output_path, "wb");
    if (!fp) {
        perror("File opening failed");
        return;
    }

    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr_write) {
        fclose(fp);
        return;
    }

    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr_write, fp);

    // Set the compression level
    png_set_compression_level(png_ptr_write, compression_level);

    // Write the image data
    png_set_IHDR(png_ptr_write, info_ptr_write, png_get_image_width(png_ptr, info_ptr),
                 png_get_image_height(png_ptr, info_ptr), png_get_bit_depth(png_ptr, info_ptr),
                 png_get_color_type(png_ptr, info_ptr), PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr_write, info_ptr_write);
    png_write_image(png_ptr_write, png_get_rows(png_ptr, info_ptr));
    png_write_end(png_ptr_write, NULL);

    fclose(fp);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
}

main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_file> <output_file> <compression_level>\n", argv[0]);
        return 1;
    }

    process_file(argv[1], argv[2], atoi(argv[3]));
    return 0;
}

