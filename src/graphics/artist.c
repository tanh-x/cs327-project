#include <stdio.h>
#include <stdbool.h>
#include "../../include/world/mapbuilder.h"

#define WHITE_BACKGROUND "\033[48;5;247m"
#define DARK_BACKGROUND "\033[48;5;240m"

#define BLUE_TEXT "\033[38;5;75m"
#define YELLOW_TEXT "\033[38;5;226m"
#define GREEN_TEXT "\033[38;5;112m"
#define DARK_GREEN_TEXT "\033[38;5;34m"
#define GRAY_TEXT "\033[38;5;232m"
#define BROWN_TEXT "\033[38;5;172m"
#define PURPLE_TEXT "\033[38;5;165m"
#define RED_TEXT "\033[38;5;196m"

#define RESET_COLOR "\033[0m"

void prettyPrint(const char *str, bool isMapColored) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (isMapColored) {
            if (c == '%') printf("%s%s", GRAY_TEXT, WHITE_BACKGROUND);
            else if (c == '.') printf("%s", YELLOW_TEXT);
            else if (c == '~') printf("%s", BLUE_TEXT);
            else if (c == ':') printf("%s", GREEN_TEXT);
            else if (c == '^') printf("%s", DARK_GREEN_TEXT);
            else if (c == '#') printf("%s", BROWN_TEXT);
            else if (c == '=') {
                    printf("%s%s#%s", GRAY_TEXT, DARK_BACKGROUND, RESET_COLOR);
                continue;
            }
            else if (c == 'M') printf("%s", PURPLE_TEXT);
            else if (c == 'C') printf("%s", RED_TEXT);
        }

        putchar(c);

        printf("%s", RESET_COLOR);
    }
}

char tileToChar(Tile *tile) {
    switch (tile->type) {
        case FLAT:
            return '.';
        case BOULDER:
            return '%';
        case BORDER:
            return '%';
        case GATE:
            return '#';
        case ROAD:
            return '#';
        case BOULDER_ROAD:
            return '=';
        case TALL_GRASS:
            return ':';
        case WATER:
            return '~';
        case TREE:
            return '^';
        case POKECENTER:
            return 'C';
        case POKEMART:
            return 'M';
        default:
            return ' ';
    }
}

void worldToString(Map *map, char *str) {
    int idx = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) str[idx++] = tileToChar(&map->tileset[y][x]);
        str[idx++] = '\n';
    }
    str[idx] = '\0';
}

