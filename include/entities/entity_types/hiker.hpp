#ifndef HIKER_H
#define HIKER_H


#include "entities/abstract_entity.hpp"

class Hiker : public AbstractEntity {
public:
    Hiker(int x, int y);

    // Hikers use gradient descent AI
    bool moveAI(Event *event) override;
};

#endif
