#ifndef ENTITIES_H
#define ENTITIES_H
// This file holds functionalities related to Entities, which are characters on the map

#include <stdbool.h>
#include "utils/heap.h"
#include "core/constants.h"
#include "utils/arraylist.h"

typedef struct GameManager GameManager;

// Enum for each character type, including the player
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


// An Entity is an object that lives only inside an EntityManager, and holds data for events on the map
// Thus, the lifespan and existence of the entity object is bound to the map.
// Entities have a type (incl. PLAYER), and have a map coordinate tuple. It also has a "soul" (see below).
// Whenever the map changes, all entities as well as the current EntityManager is cleared and disposed of.
typedef struct Entity {
    EntityType type;
    int mapX;
    int mapY;

    char* name;

    // A soul is a pointer to a fragment of additional data that some entities may have.
    // This allows us to store additional states with the entities' own structs (e.g. Player)
    // [ This is a coping mechanism with the lack of OOP constructs. ]
    void* soul;

    // Whether the player can battle this NPC. Entities that can't be fought or has already been defeated will
    // have this value as false
    bool activeBattle;
} Entity;


// The EventManager stores the current state of entities on the map.
// It is always bound to the GameManager singleton, but is cleared and disposed of every time we switch to a new map,
// or whenever the current map gets disposed of.
typedef struct EntityManager {
    // An array list of entities currently in use
    ArrayList* entities;
    // A 2D array of nullable Entity pointers, which allows the game to know whether there's an entity at a specific
    // position on the map. The 2D array has the same capacity as the map.
    Entity* entMap[MAP_HEIGHT][MAP_WIDTH];
    // A heap that contains queued Event objects, sorted by the time at which they will be resolved.
    heap_t* eventQueue;
} EntityManager;


// Creates a new entity of a specified type and place it on the map and the current EntityManager.
// Returns the pointer to that Entity, or null if it failed. If a NULL is returned, no side effects have been made.
Entity* spawnEntity(EntityType type, int x, int y);

// Instantiates a new soul for the corresponding Entity.
// Should be called everytime an entity with a soul is spawned, as it handles the soul creation for us.
void* constructCharacterSoul(Entity* entity);

// Initializes a new EntityManager and assign it to the game->entManager pointer, and also returns it.
// Should be called when loading a new map.
EntityManager* initializeEntityManager(int initialNumEntities);

// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
// If it was unsuccessful (false return), it will be one of the three following scenarios:
// - The EntityManager or the Entity was falsy/null
// - The specified position was out of bounds
// - The tile is already occupied by another entity
bool moveEntity(Entity* entity, int dx, int dy);

// Frees all dynamically allocated memory associated with the EntityManager, which includes:
// * The eventQueue heap
// * The 2D Entity array
void disposeEntityManager(EntityManager* entManager);

// Frees the memory allocated to the given entity
void disposeEntity(Entity* entity);

#endif
