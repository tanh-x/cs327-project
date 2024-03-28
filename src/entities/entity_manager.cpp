#include "entities/entity_manager.hpp"
#include "core/game.hpp"
#include "entities/entity_types/player_vessel.hpp"
#include "entities/pathfinding.hpp"

// Initializes a new EntityManager and assign it to the game->currentEntManager pointer, and also returns it.
// Should be called when loading a new map.
EntityManager::EntityManager() {
    // Initialize the entity list
//    this->entities = constructArrayList(initialNumEntities + ENTITIES_INITIAL_CAPACITY_PADDING);

    // Initialize the entity map
    for (auto &row: this->entMap) {
        for (AbstractEntity*&ent: row) {
            ent = nullptr;
        }
    }

    // Initialize the eventQueue heap
    this->eventQueue = nullptr;
    reinitializeEventQueue();

    this->eventTime = 0;
    this->nextBattleInitiationTime = 0;

    // Add the player to the entity manager
    auto* playerEnt = new PlayerVessel(GAME.player->mapX, GAME.player->mapY);
    this->addEntity(playerEnt);

    // Point the entity field in the Player singleton towards this new entity
    GAME.player->currentEntity = playerEnt;
    // As well as the vesselEntity pointer in the manager
    this->vesselEntity = playerEnt;
}

// Frees all dynamically allocated memory associated with the EntityManager, which includes:
// * The eventQueue heap
// * The 2D AbstractEntity array
EntityManager::~EntityManager() {
    // Free every entity in the entity manager
//    for (int i = 0; i < currentEntManager->entities->size; i++) {
//        auto* ent = static_cast<AbstractEntity*>(currentEntManager->entities->array[i]);
//        free(ent);
//    }

    // Free the entity array list
    entities.clear();

    // Free the event queue
    heap_delete(eventQueue);
    free(eventQueue);
    eventQueue = nullptr;

    // Free the entity map
//    free(currentEntManager->entMap);

//    free(currentEntManager);
}


void EntityManager::reinitializeEventQueue() {
    if (eventQueue != nullptr) heap_delete(eventQueue);

    // Create a new heap
    this->eventQueue = static_cast<heap_t*>(malloc(sizeof(heap_t)));
    heap_init(this->eventQueue, eventComparator, disposeEvent);
}

void EntityManager::fillFirstTurns() {
    for (auto &entity: entities) {
        // If it was a player, simply add the first input blocking event
        if (entity->type == EntityEnum::PLAYER) {
            enqueueInputBlockingEvent(0);
            continue;
        }

        // Try creating and queueing a new event.
        Event* event = entity->constructEventOnTurn();
        if (event == nullptr) continue;

        // Event initialization was successful, so we add it to the queue
        enqueueEvent(event);
    }
}


void EntityManager::addEntity(AbstractEntity* entity) {
    int x = entity->mapX;
    int y = entity->mapY;

    // Don't spawnNPC the entity if the current cell is already occupied
    if (entMap[y][x] != nullptr) return;

    // Add it to the entity map
    entMap[y][x] = entity;

    // Add it to the entity list
    entities.push_back(entity);
}

void EntityManager::spawnTrainers(Map* map, int numTrainers) {
    // Possible trainer types
    EntityEnum types[] = {
        EntityEnum::HIKER,
        EntityEnum::RIVAL,
        EntityEnum::PACER,
        EntityEnum::WANDERER,
        EntityEnum::SENTRY,
        EntityEnum::EXPLORER
    };
    int numTypes = 6;

    // Start placing numTrainers NPCs, or until we stop prematurely if it got too crowded
    for (int i = 0; i < numTrainers; i++) {
        EntityEnum entType;
        AbstractEntity* entity = nullptr;

        // Keep retrying to place the trainer until we land on a valid spot
        for (int _ = 0; _ < MAX_ITERATIONS && entity == nullptr; _++) {
            // Get the type of the new trainer
            if (i == 0) entType = EntityEnum::HIKER;  // First entity always a hiker
            else if (i == 1) entType = EntityEnum::RIVAL;  // Second entity always a rival
            else entType = types[randomInt(0, numTypes - 1)];

            // Don't spawnNPC the NPC on the border or right next to it
            // While we can do (1, -2) instead, it's just my preference to leave a 1 tile padding
            int x = randomInt(2, MAP_WIDTH - 3);
            int y = randomInt(2, MAP_HEIGHT - 3);

            // Check if the terrain cost was infinite
            if (getTerrainCost(map->tileset[y][x].type, entType) == UNCROSSABLE) continue;

            // If all previous checks were successful, then try spawning a new NPC
            entity = AbstractEntity::spawnNPC(entType, x, y);
            if (entity != nullptr) addEntity(entity);
            // spawnEntity might return NULL, indicating an unsuccessful placement
            // if so then entity = nullptr and we try again
        }

        // If we already went through MAX_ITERATIONS and entity is still NULL, give up
        if (entity == nullptr) break;

        // Else, we got a successful trainer placement.

        // Make them follow and try to fight the player
        entity->activeBattle = true;
    }
}

// Adds the given event into the EntityManager's event queue.
void EntityManager::enqueueEvent(Event* event) const {
    event->resolveTime = eventTime + event->cost;
    heap_insert(eventQueue, event);
}
