#ifndef EXPLORER_H
#define EXPLORER_H

#include "utils/mathematics.hpp"

// Explorers walk in one direction, and turns in a random direction if they can't walk forward.
class Explorer : public Entity {
public:
    Explorer(int x, int y);

    bool moveAI(Event* event) override;

private:
    // A 2D int vector that stores the current walking direction
    Int2D walk;
};

#endif
