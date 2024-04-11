#ifndef PACER_H
#define PACER_H

#include "utils/mathematics.hpp"
#include "entities/corporeal_entity.hpp"

// Pacers walk in one direction, then turn the opposite way if they hit an uncrossable tile
class Pacer : public CorporealEntity {
public:
    Pacer(int x, int y);

    bool moveAI(Event* event) override;

private:
    // A 2D int vector that stores the current walking direction
    Int2D walk {};
};

#endif
