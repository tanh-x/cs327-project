#ifndef CORPOREAL_ENTITY_H
#define CORPOREAL_ENTITY_H

#include "abstract_entity.hpp"
#include "entities/entity_types/entity_enum.hpp"
#include "entities/event.hpp"

// A CorporealEntity is an entity that has a physical position on the map and lives on the map
//
// Whenever the map changes, all corporeal entities will still be stored in current EntityManager,
// and will be inactive until the map is loaded again.
class CorporealEntity : public AbstractEntity {
public:
    // The position of the entity on the map
    int mapX;
    int mapY;

    CorporealEntity(EntityEnum type, int x, int y);

    // Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
    // Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
    // If it was unsuccessful (false return), it will be one of the three following scenarios:
    // - The EntityManager was falsy/null
    // - The specified position was out of bounds
    // - The tile is already occupied by another entity
    bool moveBy(int dx, int dy);

    static CorporealEntity* spawnNpc(EntityEnum type, int mapX, int mapY);

    // Creates a new event according to the entity's AI
    Event* constructEventOnTurn();

    // A movement AI is a function that takes in an event pointer argument, which should be a placeholder event.
    // It returns a boolean that indicates whether the event creation was successful or not.
    //
    // The event will be of movement type, with dx, dy, and cost correctly written into.
    virtual bool moveAI(Event* event) = 0;
};

#endif
