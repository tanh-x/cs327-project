#include "entities/entity_types/hiker.hpp"
#include "entities/ai/map_ai.hpp"

Hiker::Hiker(int x, int y) : CorporealEntity(EntityEnum::HIKER, x, y) {}

// Hikers use gradient descent AI
bool Hiker::moveAI(Event* event) {
    return gradientDescentAI(event, this);
}
