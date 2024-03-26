#ifndef HIKER_H
#define HIKER_H


#include "entity/entities.hpp"

class Hiker : public Entity {
public:
    Hiker(int x, int y);

    // Hikers use gradient descent AI
    bool moveAI(Event *event) override;
};

#endif
