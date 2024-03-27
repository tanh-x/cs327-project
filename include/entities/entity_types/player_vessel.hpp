#ifndef PLAYER_VESSEL_H
#define PLAYER_VESSEL_H


#include "entities/abstract_entity.hpp"

class PlayerVessel : public AbstractEntity {
public:
    PlayerVessel(int x, int y);

    bool moveAI(Event* event) override { return false; }
};


#endif
