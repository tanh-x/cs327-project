#ifndef EXPLORER_H
#define EXPLORER_H

#include "entities/corporeal_entity.hpp"

// Explorers walk in one direction, and turns in a random direction if they can't walk forward.
class Explorer : public CorporealEntity {
public:
    Explorer(int x, int y);

    bool moveAI(Event* event) override;

private:
    // A 2D int vector that stores the current walking direction
    Int2D walk {};
};

#endif
