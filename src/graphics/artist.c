#include <stdio.h>
#include <stdbool.h>
#include "world/mapbuilder.h"

#define WHITE_BACKGROUND "\033[48;5;247m"
#define DARK_BACKGROUND "\033[48;5;240m"

#define WHITE_TEXT "\033[38;5;252m"
#define LIGHT_GRAY_TEXT "\033[38;5;247m"
#define BLUE_TEXT "\033[38;5;75m"
#define FLAT_COLOR "\033[38;5;155m"
#define GRASS_COLOR "\033[38;5;40m"
#define TREE_COLOR "\033[38;5;28m"
#define GRAY_TEXT "\033[38;5;232m"
#define BROWN_TEXT "\033[38;5;172m"
#define PURPLE_TEXT "\033[38;5;165m"
#define RED_TEXT "\033[38;5;196m"
#define JOULDER_COLOR "\033[38;5;216m"

#define RESET_COLOR "\033[0m"

void prettyPrint(const char *str, bool isMapColored) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (isMapColored) {
            if (c == '%') printf("%s", WHITE_TEXT);
            else if (c == '&') printf("%s%s", GRAY_TEXT, WHITE_BACKGROUND);
            else if (c == '.') printf("%s", FLAT_COLOR);
            else if (c == '~') printf("%s", BLUE_TEXT);
            else if (c == ':') printf("%s", GRASS_COLOR);
            else if (c == '^') printf("%s", TREE_COLOR);
            else if (c == '#') printf("%s", BROWN_TEXT);
            else if (c == '=') {
                printf("%s#%s", LIGHT_GRAY_TEXT, RESET_COLOR);
                continue;
            } else if (c == 'M') printf("%s", PURPLE_TEXT);
            else if (c == 'C') printf("%s", RED_TEXT);
            else if (c == 'J') printf("%s", JOULDER_COLOR);
        } else {
            if (c == '=') {
                printf("#");
                continue;
            }
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
        case BORDER:
            return '%';
        case MOUNTAIN:
            return '&';
        case GATE:
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
        case JOULDER:
            return 'J';
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

