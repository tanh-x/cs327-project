#ifndef SENTRY_HPP
#define SENTRY_HPP


#include "entity/entities.hpp"

class Sentry : public Entity {
public:
    Sentry(int x, int y);

    bool moveAI(Event* event) override;
};


#endif
