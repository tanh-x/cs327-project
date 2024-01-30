#include <stdbool.h>
#include <stdlib.h>
#include "../../include/world/mapbuilder.h"
#include "../../include/utils/mathematics.h"
#include "../../include/graphics/parse_frame.h"

void initializeMap(Map *map, bool useBadApple) {
    int westGateY = 8;
    int eastGateY = 16;
    int northGateX = 35;
    int southGateX = 62;


    // Populate natural tile types
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            map->tiles[y][x].type = FLAT;
        }
    }

    if (useBadApple) {
        int **frame = parse_frame("assets/badapple-test.png");
        for (int y = 1; y < MAP_HEIGHT - 1; y++) {
            for (int x = 1; x < MAP_WIDTH - 1; x++) {
                if (frame[y - 1][x - 1] > 128) map->tiles[y][x].type = BOULDER;
                else if (map->tiles[y][x].type == BOULDER) map->tiles[y][x].type = TREE;
            }
        }

        // Clean up?
        for (int i = 0; i < MAP_HEIGHT - 2; i++) free(frame[i]);
        free(frame);
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
        map->tiles[0][x].type = BORDER;
        map->tiles[MAP_HEIGHT - 1][x].type = BORDER;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        map->tiles[y][0].type = BORDER;
        map->tiles[y][MAP_WIDTH - 1].type = BORDER;
    }

    map->tiles[westGateY][0].type = GATE;
    map->tiles[eastGateY][MAP_WIDTH - 1].type = GATE;
    map->tiles[0][northGateX].type = GATE;
    map->tiles[MAP_HEIGHT - 1][southGateX].type = GATE;
}
