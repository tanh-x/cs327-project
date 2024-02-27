#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int globalX;
    int globalY;
    int mapX;
    int mapY;
} Player;

void playerWalk(Player* player, int dx, int dy);


#endif
