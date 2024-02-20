#include <stdio.h>
#include <stdbool.h>
#include "world/mapbuilder.h"
#include "core/game.h"

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
#define JOULDER_COLOR "\033[38;5;216m"
#define RED_TEXT "\033[38;5;196m"
#define BLACK_TEXT "\033[38;5;16m"
#define INTENSE_RED_BACKGROUND "\033[48;5;196m"

#define RESET_COLOR "\033[0m"

void prettyPrint(const char *str, bool isMapColored) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        if (isMapColored) {
            switch (c) {
                case '%':
                    printf(WHITE_TEXT);
                    break;
                case '&':
                    printf("%s%s", GRAY_TEXT, WHITE_BACKGROUND);
                    break;
                case '.':
                    printf(FLAT_COLOR);
                    break;
                case '~':
                    printf(BLUE_TEXT);
                    break;
                case ':':
                    printf(GRASS_COLOR);
                    break;
                case '^':
                    printf(TREE_COLOR);
                    break;
                case '#':
                    printf(BROWN_TEXT);
                    break;
                case '=':
                    printf("%s#%s", LIGHT_GRAY_TEXT, RESET_COLOR);
                    continue;
                case 'M':
                    printf(PURPLE_TEXT);
                    break;
                case 'C':
                    printf(RED_TEXT);
                    break;
                case '@':
                    printf("%s%s", BLACK_TEXT, INTENSE_RED_BACKGROUND);
                    break;
                case 'J':
                    printf(JOULDER_COLOR);
                    break;
                case ' ':
                    printf(INTENSE_RED_BACKGROUND);
                    break;
                default:
                    break;
            }
        } else {
            if (c == '=') {
                printf("#");
                continue;
            }
        }

        putchar(c);

        printf(RESET_COLOR);
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
        default:
            return ' ';
    }
}

void worldToString(GameManager *game, char *str) {
    Player *player = game->player;
    Map *currentMap = game->world->maps[player->globalY + WORLD_Y_SPAN][player->globalX + WORLD_X_SPAN];

    int idx = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            str[idx++] = tileToChar(&currentMap->tileset[y][x]);
        }
        str[idx++] = '\n';
    }

    // Place the player
    // + 1 accounts for the \n at the end of every line
    str[player->mapY * (MAP_WIDTH + 1) + player->mapX] = '@';

    str[idx] = '\0';
}

