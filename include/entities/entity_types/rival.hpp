#ifndef RIVAL_H
#define RIVAL_H

#include "entities/corporeal_entity.hpp"

class Rival : public CorporealEntity {
public:
    Rival(int x, int y);

    // Rivals use gradient descent AI
    bool moveAI(Event* event) override;
};


#endif
