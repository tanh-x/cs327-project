#ifndef EVENT_H
#define EVENT_H

#include "entities.h"

typedef struct Entity Entity;

typedef enum __attribute__ ((__packed__)) {
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

// Default implementation for Entity->onEvent
void resolveEvent(EntityManager *entManager, Event *event);

#endif