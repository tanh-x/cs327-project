#ifndef SENTRY_HPP
#define SENTRY_HPP


#include "entities/abstract_entity.hpp"

class Sentry : public AbstractEntity {
public:
    Sentry(int x, int y);

    bool moveAI(Event* event) override;
};


#endif
