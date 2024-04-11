#ifndef PLAYER_VESSEL_H
#define PLAYER_VESSEL_H

#include "entities/corporeal_entity.hpp"

class PlayerVessel : public CorporealEntity {
public:
    PlayerVessel(int x, int y);

    bool moveAI(Event* event) override { return false; }
};


#endif
