#ifndef EVENT_H
#define EVENT_H
// Events indicate entity actions on the map, such as movement

#include <malloc.h>
#include "abstract_entity.hpp"

class AbstractEntity;

typedef enum __attribute__ ((__packed__)) {
    // IDLE events are placeholders, or waiting turns.
    IDLE,
    // MOVEMENT events move the entity in the direction specified by (dx, dy)
    MOVEMENT,
    // PLAYER_INPUT BLOCKING events indicate when the player can take the next turn
    PLAYER_INPUT_BLOCKING
} EventType;


// An Event is an action that will be enacted by an entity in some amount of time in the future.
class Event {
public:
    EventType type;

    // The entity carrying out this event.
    AbstractEntity* actor;

    // The coordinates relative to the actor entity at which the event will happen on.
    // For MOVEMENT, this is simply where the entity will move to.
    int dx;
    int dy;

    // The time cost for this event.
    int cost;

    // The cost plus time at which this event was queued.
    // This value will be written into during an EntityManager::enqueueEvent call.
    int resolveTime;
};


// Comparator for the event's resolveTime, for purposes of using the event queue
int eventComparator(const void* event, const void* other);

// Resolves the event by carrying out the specified action.
// Called on the event when it is polled from the event queue.
void resolveEvent(Event* event);

// Frees the currently allocated memory for the event.
void disposeEvent(void* event);

// Instantiates an IDLE event.
Event* constructIdleEvent(AbstractEntity* entity, int cost);

// Instantiates a PLAYER_INPUT_BLOCKING event
void enqueueInputBlockingEvent(int delay);

#endif