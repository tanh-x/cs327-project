#ifndef MAP_AI_H
#define MAP_AI_H

#include "event.h"
#include "world/mapbuilder.h"
#include "core/player.h"

Event *constructEventOnTurn(Map *map, Player *player, Entity *entity);

bool (*dispatchMovementAIHandler(EntityType type))(Event *event, Map *map, Player *player, Entity *entity);

#endif
