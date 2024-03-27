// Events indicate entity actions on the map, such as movement
#include <cstdlib>
#include "entity/event.hpp"
#include "core/game_manager.hpp"

// Comparator for the event's resolveTime, for purposes of using the event queue
int eventComparator(const void* event, const void* other) {
    return ((Event*) event)->resolveTime - ((Event*) other)->resolveTime;
}

// Resolves the event by carrying out the specified action.
// Called on the event when it is polled from the event queue.
void resolveEvent(Event* event) {
    Entity* entity = event->actor;
    switch (event->type) {
        case MOVEMENT: entity->moveBy(event->dx, event->dy);
            break;
        case IDLE: break;
        case PLAYER_INPUT_BLOCKING: exit(1);  // This event should not be delegated to the manager
    }
}

// Frees the currently allocated memory for the event.
void disposeEvent(void* event) {
    free(event);
}

// Instantiates an IDLE event.
Event* constructIdleEvent(Entity* entity, int cost) {
    auto* event = static_cast<Event*>(malloc(sizeof(Event)));
    event->type = IDLE;
    event->dx = 0;
    event->dy = 0;
    event->cost = cost;
    event->actor = entity;
    return event;
}


// Instantiates a PLAYER_INPUT_BLOCKING event
void enqueueInputBlockingEvent(int delay) {
    Event* event = constructIdleEvent(GAME.player->currentEntity, delay);
    event->type = PLAYER_INPUT_BLOCKING;
    GAME.currentEntManager->enqueueEvent(event);
}