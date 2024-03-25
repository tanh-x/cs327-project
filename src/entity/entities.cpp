// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.hpp"
#include "entity/entities.hpp"
#include "entity/event.hpp"
#include "core/player.hpp"
#include "entity/npc/pacer.hpp"
#include "entity/npc/wanderer.hpp"
#include "entity/npc/explorer.hpp"

#define ENTITIES_INITIAL_CAPACITY_PADDING 6

const char* trainerNames[] = TRAINER_NAMES;

// Creates a new entity of a specified type and place it on the map and the current EntityManager.
// Returns the pointer to that Entity, or null if it failed. If a NULL is returned, no side effects have been made.
Entity* spawnEntity(EntityType type, int x, int y) {
    EntityManager* entManager = GAME.entManager;
    // Don't spawn the entity if the current cell is already occupied
    if (entManager->entMap[y][x] != nullptr) return nullptr;

    // Else, create the entity
    auto* entity = static_cast<Entity*>(malloc(sizeof(Entity)));
    entity->type = type;
    entity->mapX = x;
    entity->mapY = y;
    entManager->entMap[y][x] = entity;
    entity->soul = constructCharacterSoul(entity);

    // If they're an NPC, give them a random name
    if (type != PLAYER) {
        entity->name = trainerNames[randomInt(0, NUM_TRAINER_NAMES - 1)];
    } else {
        // TODO: forgor
        entity->name = "Player";
    }

    // Add it to the entity list, and then return it
    arrayList_insert(entManager->entities, entity);
    return entity;
}

// Instantiates a new soul for the corresponding Entity.
// Should be called everytime an entity with a soul is spawned, as it handles the soul creation for us.
void* constructCharacterSoul(Entity* entity) {
    switch (entity->type) {
        case PLAYER: return GAME.player;
        case PACER: return constructPacerSoul();
        case WANDERER: return constructWandererSoul(GAME.world->current->tileset[entity->mapY][entity->mapX].type);
        case EXPLORER: return constructExplorerSoul();
        default: return nullptr;
    }
}

// Initializes a new EntityManager and assign it to the GAME.entManager pointer, and also returns it.
// Should be called when loading a new map.
EntityManager* initializeEntityManager(int initialNumEntities) {
    auto* entManager = static_cast<EntityManager*>(malloc(sizeof(EntityManager)));
    GAME.entManager = entManager;

    // Initialize the entity list
    entManager->entities = constructArrayList(initialNumEntities + ENTITIES_INITIAL_CAPACITY_PADDING);

    // Initialize the entity map
    for (auto &i: entManager->entMap) {
        for (Entity*&j: i) {
            j = nullptr;
        }
    }

    // Initialize the eventQueue heap
    entManager->eventQueue = static_cast<heap_t*>(malloc(sizeof(heap_t)));
    heap_init(entManager->eventQueue, eventComparator, disposeEvent);

    // Add the player to the entManager
    Entity* playerEnt = spawnEntity(PLAYER, GAME.player->mapX, GAME.player->mapY);

    // Point the entity field in the Player singleton towards this new entity
    GAME.player->currentEntity = playerEnt;

    // Add an input event for the player that resolves immediately
    enqueueInputBlockingEvent(0);

    return entManager;
}

// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
// If it was unsuccessful (false return), it will be one of the three following scenarios:
// - The EntityManager or the Entity was falsy/null
// - The specified position was out of bounds
// - The tile is already occupied by another entity
bool moveEntity(Entity* entity, int dx, int dy) {
    EntityManager* entManager = GAME.entManager;
    if (!entManager || !entity) return false;

    int newX = entity->mapX + dx;
    int newY = entity->mapY + dy;

    // Boundary check for new position
    if (!isInsideMapBounds(newX, newY)) return false;

    // Check if the new position is occupied
    if (entManager->entMap[newY][newX] != nullptr) return false;

    // If we got here, it will be a valid move, nothing should've been mutated before this point

    // Move entity in the entMap
    entManager->entMap[entity->mapY][entity->mapX] = nullptr;
    entity->mapX = newX;
    entity->mapY = newY;
    entManager->entMap[newY][newX] = entity;

    // Update player position if entity is a player
    if (entity->type == PLAYER && entity->soul != nullptr) {
        Player* player = (Player*) entity->soul;
        player->mapX = newX;
        player->mapY = newY;
    }

    return true;
}

// Frees all dynamically allocated memory associated with the EntityManager, which includes:
// * The eventQueue heap
// * The 2D Entity array
void disposeEntityManager(EntityManager* entManager) {
    // Free every entity in the entity manager
    for (int i = 0; i < entManager->entities->size; i++) {
        auto* ent = static_cast<Entity*>(entManager->entities->array[i]);
        disposeEntity(ent);
    }

    // Free the entity array list
    arrayList_free(entManager->entities);
    entManager->entities = nullptr;

    // Free the event queue
    heap_delete(entManager->eventQueue);
    free(entManager->eventQueue);
    entManager->eventQueue = nullptr;

    // Free the entity map
//    free(entManager->entMap);

    free(entManager);
}


void disposeEntity(Entity* entity) {
    if (entity->type == PLAYER) {
        // Remove the entity reference from the Player singleton
        auto* player = static_cast<Player*>(entity->soul);
        if (player != nullptr) player->currentEntity = nullptr;

    } else if (entity->soul != nullptr) {
        // Otherwise, just free the entity's soul
        free(entity->soul);
        entity->soul = nullptr;
    }

    free(entity);
}