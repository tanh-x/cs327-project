#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "core/constants.hpp"
#include "entities.hpp"
#include "utils/arraylist.hpp"
#include "utils/heap.h"

// The EventManager stores the current state of entities on the map.
// It is always bound to the GameManager singleton, but is cleared and disposed of every time we switch to a new map,
// or whenever the current map gets disposed of.
class EntityManager {
public:
    // Initializes a new EntityManager and assign it to the game->entManager pointer, and also returns it.
    // Should be called when loading a new map.
    explicit EntityManager(int initialNumEntities);

    // Frees all dynamically allocated memory associated with the EntityManager, which includes:
    // * The eventQueue heap
    // * The 2D Entity array
    ~EntityManager();

    // An array list of entities currently in use
    ArrayList* entities;
    // A 2D array of nullable Entity pointers, which allows the game to know whether there's an entity at a specific
    // position on the map. The 2D array has the same capacity as the map.
    Entity* entMap[MAP_HEIGHT][MAP_WIDTH] {};
    // A heap that contains queued Event objects, sorted by the time at which they will be resolved.
    heap_t* eventQueue;

    void addEntity(Entity* entity);
};

#endif