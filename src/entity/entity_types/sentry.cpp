#include "entity/entity_types/sentry.hpp"

Sentry::Sentry(int x, int y) : Entity(EntityEnum::SENTRY, x, y) {}

bool Sentry::moveAI(Event* event) {
    event->type = IDLE;
    event->cost = 500;  // Arbitrarily large value
    return true;
}
