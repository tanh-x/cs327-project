#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/graphics/artist.h"

#define CLEAR_SCREEN "\033[2J\033[H"

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

int main(int argc, char *argv[]) {
    printf("%s", CLEAR_SCREEN);
    bool doColoring = (argc > 1 && strcmp(argv[1], "--color") == 0);

    prettyPrint("Hello, World!~~%%\n", doColoring);

    return 0;
}
