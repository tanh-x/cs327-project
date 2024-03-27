#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "vector"
#include "core/constants.hpp"
#include "entities.hpp"
#include "utils/arraylist.hpp"
#include "utils/heap.h"
#include "entity/entity_types/player_vessel.hpp"
#include "world/mapbuilder.hpp"

class Map;

// The EventManager stores the current state of entities on the map.
// It is always bound to the GameManager singleton, but is cleared and disposed of every time
// we switch to a new map, or whenever the current map gets disposed of.
class EntityManager {
public:
    // Initializes a new EntityManager and assign it to the game->currentEntManager pointer, and also returns it.
    // Should be called when loading a new map.
    EntityManager();

    // Frees all dynamically allocated memory associated with the EntityManager, which includes:
    // * The eventQueue heap
    // * The 2D Entity array
    ~EntityManager();

    int eventTime;

    // Used to prevent NPCs from spamming too many battle initiations with the player
    int nextBattleInitiationTime;

    // List of entities
    std::vector<Entity*> entities;

    // Pointer to the unique player vessel entity in the entity manager
    PlayerVessel* vesselEntity;

    // A 2D array of nullable Entity pointers, which allows the game to know whether there's an entity at a specific
    // position on the map. The 2D array has the same capacity as the map.
    Entity* entMap[MAP_HEIGHT][MAP_WIDTH] {};

    // A heap that contains queued Event objects, sorted by the time at which they will be resolved.
    heap_t* eventQueue;

    void addEntity(Entity* entity);

    void spawnTrainers(Map* map, int numTrainers);

    // Adds the given event into the EntityManager's event queue.
    void enqueueEvent(Event* event) const;
};

#endif