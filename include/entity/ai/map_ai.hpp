#ifndef MAP_AI_H
#define MAP_AI_H
// Templates for AI functionalities for map events

#include "entity/event.hpp"
#include "world/mapbuilder.hpp"
#include "core/player.hpp"

bool gradientDescentAI(Event* event, Entity* entity);

// Creates a new event according to the entity's AI
//Event* constructEventOnTurn(Entity* entity);

// Takes in an EntityEnum, and returns a pointer to a function, called a movement AI handler, corresponding to the
// given entity type's movement AI.
// A movement AI handler is a function that takes in 4 arguments: event, map, player, entity; and returns a boolean
// that indicates whether the event creation was successful or not.
// The event will be of movement type, with dx, dy, and cost correctly written into.
//bool (* dispatchMovementAIHandler(EntityEnum type))(Event* event, Entity* entity);

#endif
