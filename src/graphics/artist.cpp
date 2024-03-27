#include <stdio.h>
#include "world/mapbuilder.hpp"
#include "core/game.hpp"

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
#define LIGHT_ORANGE_COLOR "\033[38;5;216m"
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
                case 'J':
                    printf(LIGHT_ORANGE_COLOR);
                    break;
                case '@':
                    printf("%s%s", BLACK_TEXT, INTENSE_RED_BACKGROUND);
                    break;
                case ' ':
                    printf(INTENSE_RED_BACKGROUND);
                    break;
                default:
                    printf("%s", LIGHT_ORANGE_COLOR);
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
        case MOUNTAIN:
            return '%';
        case BORDER:
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
            return '%';
        default:
            return ' ';
    }
}

char entityToChar(Entity *entity) {
    switch (entity->type) {
        case EntityEnum::PLAYER:
            return '@';
        case EntityEnum::HIKER:
            return 'h';
        case EntityEnum::RIVAL:
            return 'r';
        case EntityEnum::SWIMMER:
            return 'm';
        case EntityEnum::PACER:
            return 'p';
        case EntityEnum::WANDERER:
            return 'w';
        case EntityEnum::SENTRY:
            return 's';
        case EntityEnum::EXPLORER:
            return 'e';
        default:
            return '?';
    }
}

void worldToString(char *str) {
    Map* currentMap = GAME.world->current;
    EntityManager* entManager = GAME.currentEntManager;

    int idx = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (entManager->entMap[y][x] != NULL) {
                str[idx++] = entityToChar(entManager->entMap[y][x]);
            } else {
                str[idx++] = tileToChar(&currentMap->tileset[y][x]);
            }
        }
        str[idx++] = '\n';
    }
    str[idx] = '\0';
}

