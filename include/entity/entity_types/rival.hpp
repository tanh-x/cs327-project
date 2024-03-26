#ifndef RIVAL_H
#define RIVAL_H


#include "entity/entities.hpp"

class Rival : public Entity {
public:
    Rival(int x, int y);

    // Rivals use gradient descent AI
    bool moveAI(Event* event) override;
};


#endif
