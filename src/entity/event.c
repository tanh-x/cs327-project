
#include "entity/event.h"

int eventComparator(const void *this, const void *other) {
    return ((Event *) this)->resolveTime - ((Event *) other)->resolveTime;
}

void enqueueEvent(EntityManager *entManager, Event *event) {
    heap_insert(entManager->eventQueue, event);
}

void resolveEvent(EntityManager *entManager, Event *event) {
    Entity *entity = event->actor;
    switch (event->type) {
        case MOVEMENT:
            moveEntity(entManager, entity, event->dx, event->dy);
            break;
    }

    // TODO: Add a new event into the queue for the next turn
}

void disposeEvent(void *event) {
    free((Event *) event);
}