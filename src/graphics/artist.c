#include <stdio.h>
#include <stdbool.h>
#include "../../include/world/mapbuilder.h"

#define WHITE_BACKGROUND "\033[48;5;248m"
#define DARK_BACKGROUND "\033[48;5;236m"

#define BLUE_TEXT "\033[38;5;75m"
#define LIME_TEXT "\033[38;5;191m"
#define GREEN_TEXT "\033[38;5;76m"
#define DARK_GREEN_TEXT "\033[38;5;22m"
#define GRAY_TEXT "\033[90m"
#define BROWN_TEXT "\033[38;5;172m"

#define RESET_COLOR "\033[0m"

void prettyPrint(const char *str, bool isMapColored) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (isMapColored) {
            if (c == '%') printf("%s%s", GRAY_TEXT, WHITE_BACKGROUND);
            else if (c == '.') printf("%s", LIME_TEXT);
            else if (c == '~') printf("%s", BLUE_TEXT);
            else if (c == ':') printf("%s", GREEN_TEXT);
            else if (c == '^') printf("%s", DARK_GREEN_TEXT);
            else if (c == '#') printf("%s%s", BROWN_TEXT, DARK_BACKGROUND);
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
        case TALL_GRASS:
            return ':';
        case WATER:
            return '~';
        case TREE:
            return '^';
        default:
            return ' ';
    }
}

void worldToString(Map *map, char *str) {
    int idx = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) str[idx++] = tileToChar(&map->tiles[y][x]);
        str[idx++] = '\n';
    }
    str[idx] = '\0';
}

