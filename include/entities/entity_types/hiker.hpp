#ifndef HIKER_H
#define HIKER_H

#include "entities/corporeal_entity.hpp"

class Hiker : public CorporealEntity {
public:
    Hiker(int x, int y);

    // Hikers use gradient descent AI
    bool moveAI(Event *event) override;
};

#endif
