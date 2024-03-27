#ifndef RIVAL_H
#define RIVAL_H


#include "entities/abstract_entity.hpp"

class Rival : public AbstractEntity {
public:
    Rival(int x, int y);

    // Rivals use gradient descent AI
    bool moveAI(Event* event) override;
};


#endif
