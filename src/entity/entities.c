// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.h"
#include "entity/entities.h"
#include "entity/event.h"
#include "core/player.h"
#include "entity/npc/pacer.h"
#include "entity/npc/wanderer.h"
#include "entity/npc/explorer.h"

#define ENTITIES_INITIAL_CAPACITY_PADDING 6

// Creates a new entity of a specified type and place it on the map and the current EntityManager.
// Returns the pointer to that Entity, or null if it failed. If a NULL is returned, no side effects have been made.
Entity* spawnEntity(EntityType type, int x, int y) {
    EntityManager* entManager = GAME.entManager;
    // Don't spawn the entity if the current cell is already occupied
    if (entManager->entMap[y][x] != NULL) return NULL;

    // Else, create the entity
    Entity* entity = malloc(sizeof(Entity));
    entity->type = type;
    entity->mapX = x;
    entity->mapY = y;
    entManager->entMap[y][x] = entity;
    entity->soul = constructCharacterSoul(entity);

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
        default: return NULL;
    }
}

// Initializes a new EntityManager and assign it to the GAME.entManager pointer, and also returns it.
// Should be called when loading a new map.
EntityManager* initializeEntityManager(int initialNumEntities) {
    EntityManager* entManager = malloc(sizeof(EntityManager));
    GAME.entManager = entManager;

    // Initialize the entity list
    entManager->entities = constructArrayList(initialNumEntities + ENTITIES_INITIAL_CAPACITY_PADDING);

    // Initialize the entity map
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            entManager->entMap[i][j] = NULL;
        }
    }

    // Initialize the eventQueue heap
    entManager->eventQueue = malloc(sizeof(heap_t));
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
    if (entManager->entMap[newY][newX] != NULL) {
        return false;
    }

    // If we got here, it will be a valid move, nothing should've been mutated before this point

    // Move entity in the entMap
    entManager->entMap[entity->mapY][entity->mapX] = NULL;
    entity->mapX = newX;
    entity->mapY = newY;
    entManager->entMap[newY][newX] = entity;

    // Update player position if entity is a player
    if (entity->type == PLAYER && entity->soul != NULL) {
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
        Entity* ent = entManager->entities->array[i];
        disposeEntity(ent);
    }

    // Free the entity array list
    arrayList_free(entManager->entities);
    entManager->entities = NULL;

    // Free the event queue
    heap_delete(entManager->eventQueue);
    free(entManager->eventQueue);
    entManager->eventQueue = NULL;

    // Free the entity map
//    free(entManager->entMap);

    free(entManager);
}


void disposeEntity(Entity* entity) {
    if (entity->type == PLAYER) {
        // Remove the entity reference from the Player singleton
        Player* player = entity->soul;
        if (player != NULL) player->currentEntity = NULL;

    } else if (entity->soul != NULL) {
        // Otherwise, just free the entity's soul
        free(entity->soul);
        entity->soul = NULL;
    }

    free(entity);
}