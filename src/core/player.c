#include "core/player.h"
#include "utils/mathematics.h"
#include "core/constants.h"

void playerWalk(Player* player, int dx, int dy) {
    player->mapX = clamp(player->mapX + dx, 1, MAP_WIDTH - 2);
    player->mapY = clamp(player->mapY + dy, 1, MAP_HEIGHT - 2);
}