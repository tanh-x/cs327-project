#ifndef PLAYER_VESSEL_H
#define PLAYER_VESSEL_H


#include "entity/entities.hpp"

class PlayerVessel : public Entity {
public:
    PlayerVessel(int x, int y);

    bool moveAI(Event* event) override { return false; }
};


#endif
