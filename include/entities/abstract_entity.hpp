#ifndef ENTITIES_H
#define ENTITIES_H
// This file holds functionalities related to Entities, which are characters on the map

#include "event.hpp"
#include "entities/entity_types/entity_enum.hpp"

class Event;

typedef struct GameManager GameManager;

// An AbstractEntity is an abstract class that lives inside an EntityManager, and holds data for events on the map
// Thus, the lifespan and existence of the entity object is bound to the map.
// Entities have a type (incl. PLAYER), and have a map coordinate tuple. It also has a "soul" (see below).
// Whenever the map changes, all entities as well as the current EntityManager is cleared and disposed of.
class AbstractEntity {
public:
    ~AbstractEntity();

    // The type of entity
    EntityEnum type;

    // The position of the entity on the map
    int mapX;
    int mapY;

    // A randomly generated name for the entity
    const char* name;

    // Whether the player can battle this NPC. Entities that can't be fought or has already been defeated will
    // have this value as false
    bool activeBattle;

    // Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
    // Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
    // If it was unsuccessful (false return), it will be one of the three following scenarios:
    // - The EntityManager was falsy/null
    // - The specified position was out of bounds
    // - The tile is already occupied by another entity
    bool moveBy(int dx, int dy);

    static AbstractEntity* spawnNPC(EntityEnum type, int mapX, int mapY);

    // Creates a new event according to the entity's AI
    Event* constructEventOnTurn();

    // Abstract
    virtual bool moveAI(Event* event) = 0;

protected:
    AbstractEntity(EntityEnum type, int x, int y);
};


#endif
