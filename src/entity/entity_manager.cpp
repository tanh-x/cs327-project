#include "entity/entity_manager.hpp"
#include "core/game.hpp"
#include "entity/entity_types/player_vessel.hpp"

// Initializes a new EntityManager and assign it to the game->entManager pointer, and also returns it.
// Should be called when loading a new map.
EntityManager::EntityManager(int initialNumEntities) {
    // Initialize the entity list
//    this->entities = constructArrayList(initialNumEntities + ENTITIES_INITIAL_CAPACITY_PADDING);

    // Initialize the entity map
    for (auto &row: this->entMap) {
        for (Entity*&ent: row) {
            ent = nullptr;
        }
    }

    // Initialize the eventQueue heap
    this->eventQueue = static_cast<heap_t*>(malloc(sizeof(heap_t)));
    heap_init(this->eventQueue, eventComparator, disposeEvent);

    // Add the player to the entity manager
    auto* playerEnt = new PlayerVessel(GAME.player->mapX, GAME.player->mapY);
    this->addEntity(playerEnt);

    // Point the entity field in the Player singleton towards this new entity
    GAME.player->currentEntity = playerEnt;
}

// Frees all dynamically allocated memory associated with the EntityManager, which includes:
// * The eventQueue heap
// * The 2D Entity array
EntityManager::~EntityManager() {
    // Free every entity in the entity manager
//    for (int i = 0; i < entManager->entities->size; i++) {
//        auto* ent = static_cast<Entity*>(entManager->entities->array[i]);
//        delete ent;
//    }

    // Free the entity array list
    entities.clear();

    // Free the event queue
    heap_delete(eventQueue);
    free(eventQueue);
    eventQueue = nullptr;

    // Free the entity map
//    free(entManager->entMap);

//    free(entManager);
}

void EntityManager::addEntity(Entity* entity) {
    int x = entity->mapX;
    int y = entity->mapY;

    // Don't spawnNPC the entity if the current cell is already occupied
    if (entMap[y][x] != nullptr) return;

    // Add it to the entity map
    entMap[y][x] = entity;

    // Add it to the entity list
    entities.push_back(entity);
}
