#include <png.h>
#include <cstdlib>
#include <cstdio>
#include "world/mapbuilder.hpp"

// Credit: https://stackoverflow.com/a/65590152/
// Derived from answer by user Halt State, shared under CC BY-SA 4.0
int** parse_frame(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        return nullptr;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);

    png_init_io(png, fp);
    png_read_info(png, info);

    int height = MAP_HEIGHT - 2;
    int width = MAP_WIDTH - 2;

    if (
        static_cast<int>(png_get_image_width(png, info)) != width
        || static_cast<int>(png_get_image_height(png, info)) != height
    ) {
        fprintf(stderr, "Image dimensions do not match expected capacity of %dx%d.\n", width, height);
        fclose(fp);
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    int** image = static_cast<int**>(malloc(height * sizeof(int*)));
    for (int y = 0; y < height; y++) {
        image[y] = static_cast<int*>(malloc(width * sizeof(int)));
    }

    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 3]);
            image[y][x] = px[0] + px[1] + px[2] / 3;
        }
    }

    fclose(fp);
    png_destroy_read_struct(&png, &info, nullptr);

    return image;
}