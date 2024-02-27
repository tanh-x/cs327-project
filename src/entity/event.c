
#include "entity/event.h"

int eventComparator(const void *this, const void *other) {
    return ((Event *) this)->resolveTime - ((Event *) other)->resolveTime;
}

void resolveEvent(EntityManager *entManager, Event *event) {
    Entity *entity = event->actor;
    switch (event->type) {
        case MOVEMENT:
            moveEntity(entManager, entity, event->dx, event->dy);
            break;
    }
}