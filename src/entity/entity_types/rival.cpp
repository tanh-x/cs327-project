#include "entity/entity_types//rival.hpp"
#include "entity/ai/map_ai.hpp"

Rival::Rival(int x, int y) : Entity(EntityEnum::HIKER, x, y) {}

// Hikers use gradient descent AI
bool Rival::moveAI(Event* event) {
    return gradientDescentAI(event, this);
}
