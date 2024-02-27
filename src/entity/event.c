#include "entity/event.h"

int eventComparator(const void* this, const void* other) {
    return ((Event*) this)->resolveTime - ((Event*) other)->resolveTime;
}

void enqueueEvent(EntityManager* entManager, Event* event) {
    heap_insert(entManager->eventQueue, event);
//    printf("Enqueued event from %d (%d, %d), resolves at %d\n",
//           event->actor->type,
//           event->actor->mapX,
//           event->actor->mapY,
//           event->resolveTime
//    );
}

void resolveEvent(EntityManager* entManager, Event* event) {
//    printf("Resolving event from %d (%d, %d) + (%d, %d), resolves at %d, type is %d\n",
//           event->actor->type,
//           event->actor->mapX,
//           event->actor->mapY,
//           event->dx,
//           event->dy,
//           event->resolveTime,
//           event->type
//    );
    Entity* entity = event->actor;
    switch (event->type) {
        case MOVEMENT: moveEntity(entManager, entity, event->dx, event->dy);
            break;
        case IDLE: break;
    }
}

void disposeEvent(void* event) {
    free((Event*) event);
}

Event* constructIdleEvent(Entity* entity, int cost) {
    Event* event = malloc(sizeof(Event));
    event->type = IDLE;
    event->dx = 0;
    event->dy = 0;
    event->cost = cost;
    event->actor = entity;
    return event;
}