#ifndef ENTITIES_H
#define ENTITIES_H
// This file holds functionalities related to Entities, which are characters on the map

#include <stdbool.h>
#include "utils/heap.h"
#include "core/constants.h"

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

    // This is a coping mechanism with the lack of OOP constructs.
    // A soul is a pointer to a fragment of additional data that some entities may have.
    // This allows us to store additional states with the entities' own structs (e.g. Player)
    void* soul;
} Entity;


// The EventManager stores the current state of entities on the map.
// It is always bound to the GameManager singleton, but is cleared and disposed of every time we switch to a new map,
// or whenever the current map gets disposed of.
typedef struct EntityManager {
    // A 2D array of nullable Entity pointers, which allows the game to know whether there's an entity at a specific
    // position on the map. The 2D array has the same size as the map.
    Entity* entMap[MAP_HEIGHT][MAP_WIDTH];
    // A heap that contains queued Event objects, sorted by the time at which they will be resolved.
    heap_t* eventQueue;
} EntityManager;


// Creates a new entity of a specified type and place it on the map and the current EntityManager.
// Returns the pointer to that Entity, or null if it failed. If a NULL is returned, no side effects have been made.
Entity* spawnEntity(GameManager* game, EntityType type, int x, int y);

// Instantiates a new soul for the corresponding Entity.
// Should be called everytime an entity with a soul is spawned, as it handles the soul creation for us.
void* constructCharacterSoul(Entity* entity, GameManager* game);

// Initializes a new EntityManager and assign it to the game->entManager pointer.
// Should be called when loading a new map.
void initializeEntityManager(GameManager* game);

// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
bool moveEntity(EntityManager* entManager, Entity* entity, int dx, int dy);

// Frees all dynamically allocated memory associated with the EntityManager, which includes:
// * The eventQueue heap
// * The 2D Entity array
void disposeEntityManager(EntityManager* entManager);

#endif
