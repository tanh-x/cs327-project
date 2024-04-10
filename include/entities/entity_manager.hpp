#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include "vector"
#include "core/constants.hpp"
#include "abstract_entity.hpp"
#include "utils/heap.h"
#include "entities/entity_types/player_vessel.hpp"
#include "world/mapbuilder.hpp"

class Map;

// The EventManager stores the current state of entities on the map.
// It is bound to each and every map, and goes inactive everytime the player steps out of the map.
class EntityManager {
public:
    // Initializes a new EntityManager and assign it to the game->currentEntManager pointer, and also returns it.
    // Should be called when loading a new map.
    EntityManager();

    // Frees all dynamically allocated memory associated with the EntityManager, which includes:
    // * The eventQueue heap
    // * The 2D AbstractEntity array
    ~EntityManager();

    int eventTime;

    // Used to prevent NPCs from spamming too many battle initiations with the player
    int nextBattleInitiationTime;

    // List of entities
    std::vector<AbstractEntity*> entities;

    // Pointer to the unique player vessel entity in the entity manager
    PlayerVessel* vesselEntity;

    // A 2D array of nullable AbstractEntity pointers, which allows the game to know whether there's an entity at a specific
    // position on the map. The 2D array has the same capacity as the map.
    std::array<std::array<AbstractEntity*, MAP_WIDTH>, MAP_HEIGHT> entMap {};

    // A heap that contains queued Event objects, sorted by the time at which they will be resolved.
    heap_t* eventQueue;

    void reinitializeEventQueue();

    void fillFirstTurns();

    void addEntity(AbstractEntity* entity);

    void spawnTrainers(Map* map, int numTrainers);

    // Adds the given event into the EntityManager's event queue.
    void enqueueEvent(Event* event) const;
};

#endif