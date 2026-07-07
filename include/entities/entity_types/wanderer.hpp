#ifndef WANDERER_H
#define WANDERER_H

#include "entities/corporeal_entity.hpp"

// Wanderers walk in one direction, and never leaves their spawnNpc terrain.
class Wanderer : public CorporealEntity {
public:
    Wanderer(int x, int y);

    // Wanderers move until they get to the edge of their "birthplace" terrain (stored in the soul), then they turn
    // in a random direction.
    bool moveAI(Event* event) override;

private:
    // A 2D int vector that stores the current walking direction
    Int2D walk {};
    // The type of tile that the wanderer was spawned on
    TileType birthplace;
};

#endif
