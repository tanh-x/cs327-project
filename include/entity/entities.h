#ifndef ENTITIES_H
#define ENTITIES_H

#include "world/mapbuilder.h"
#include "utils/heap.h"
#include "core/game.h"

typedef struct GameManager GameManager;

typedef enum __attribute__ ((__packed__)) {
    PLAYER,
    HIKER,
    RIVAL,
    SWIMMER,
    PACER,
    WANDERER,
    SENTRY,
    EXPLORER
} EntityType;

typedef struct Entity {
    EntityType type;
    int mapX;
    int mapY;

    // Coping mechanism with the lack of OOP constructs
    // Pointer to special entities' additional structs (e.g. Players)
    void *soul;
} Entity;

typedef struct EntityManager {
    Entity *entMap[MAP_HEIGHT][MAP_WIDTH];
    heap_t *eventQueue;
} EntityManager;

Entity *constructEntity(EntityManager *entManager, EntityType type, int x, int y);

EntityManager *instantiateEntityManager(GameManager *game);

bool moveEntity(EntityManager *entManager, Entity *entity, int dx, int dy);

void disposeEntityManager(EntityManager *entManager);

#endif
