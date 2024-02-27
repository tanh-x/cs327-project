#ifndef EVENT_H
#define EVENT_H

#include <malloc.h>
#include "entities.h"

typedef struct Entity Entity;
typedef struct EntityManager EntityManager;

typedef enum __attribute__ ((__packed__)) {
    IDLE,
    MOVEMENT,
} EventType;

typedef struct Event {
    EventType type;
    int dx;
    int dy;
    int cost;
    int resolveTime;
    Entity *actor;
} Event;

int eventComparator(const void *this, const void *other);

void enqueueEvent(EntityManager *entManager, Event *event);

// Default implementation for Entity->onEvent
void resolveEvent(EntityManager *entManager, Event *event);

void disposeEvent(void *event);

Event* constructIdleEvent(Entity* entity, int cost);


#endif