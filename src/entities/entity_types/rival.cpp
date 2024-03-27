#include "entities/entity_types//rival.hpp"
#include "entities/ai/map_ai.hpp"

Rival::Rival(int x, int y) : AbstractEntity(EntityEnum::HIKER, x, y) {}

// Hikers use gradient descent AI
bool Rival::moveAI(Event* event) {
    return gradientDescentAI(event, this);
}
