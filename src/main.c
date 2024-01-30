#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/graphics/artist.h"
#include "../include/world/mapbuilder.h"
#include "../include/utils/mathematics.h"

#define CLEAR_SCREEN "\033[2J\033[H"

int main(int argc, char *argv[]) {
    printf("%s", CLEAR_SCREEN);
    bool doColoring = (argc > 1 && strcmp(argv[1], "--color") == 0);

    Map newMap;

    initializeMap(&newMap);

    prettyPrint("Hello, World!~~%%\n", doColoring);

    return 0;
}
