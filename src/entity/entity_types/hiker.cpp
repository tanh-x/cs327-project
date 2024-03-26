#include "entity/entity_types/hiker.hpp"
#include "entity/ai/map_ai.hpp"

Hiker::Hiker(int x, int y) : Entity(EntityEnum::HIKER, x, y) {}

// Hikers use gradient descent AI
bool Hiker::moveAI(Event* event) {
    return gradientDescentAI(event, this);
}
