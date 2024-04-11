#ifndef SENTRY_HPP
#define SENTRY_HPP

#include "entities/corporeal_entity.hpp"

class Sentry : public CorporealEntity {
public:
    Sentry(int x, int y);

    bool moveAI(Event* event) override;
};


#endif
