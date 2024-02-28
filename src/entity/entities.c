// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.h"
#include "entity/entities.h"
#include "entity/event.h"
#include "core/player.h"
#include "entity/npc/pacer.h"
#include "entity/npc/wanderer.h"

// Creates a new entity of a specified type and place it on the map and the current EntityManager.
// Returns the pointer to that Entity, or null if it failed. If a NULL is returned, no side effects have been made.
Entity* spawnEntity(GameManager* game, EntityType type, int x, int y) {
    EntityManager* entManager = game->entManager;
    // Don't spawn the entity if the current cell is already occupied
    if (entManager->entMap[y][x] != NULL) return NULL;

    // Else, create the entity and return it
    Entity* entity = malloc(sizeof(Entity));
    entity->type = type;
    entity->mapX = x;
    entity->mapY = y;
    entManager->entMap[y][x] = entity;
    entity->soul = constructCharacterSoul(entity, game);
    return entity;
}

// Instantiates a new soul for the corresponding Entity.
// Should be called everytime an entity with a soul is spawned, as it handles the soul creation for us.
void* constructCharacterSoul(Entity* entity, GameManager* game) {
    switch (entity->type) {
        case PLAYER: return game->player;
        case PACER: return constructPacerSoul();
        case WANDERER: return constructWandererSoul(game->world->current->tileset[entity->mapY][entity->mapX].type);
        default: return NULL;
    }
}

// Initializes a new EntityManager and assign it to the game->entManager pointer.
// Should be called when loading a new map.
void initializeEntityManager(GameManager* game) {
    EntityManager* entManager = malloc(sizeof(EntityManager));
    game->entManager = entManager;

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
    Entity* playerEnt = spawnEntity(game, PLAYER, game->player->mapX, game->player->mapY);

    // Add an empty event for the player
    Event* event = constructIdleEvent(playerEnt, 0);
    event->resolveTime = 0;
    enqueueEvent(entManager, event);
}

// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
bool moveEntity(EntityManager* entManager, Entity* entity, int dx, int dy) {
    if (!entManager || !entity) return false;

    // Boundary check for new position
    int newX = entity->mapX + dx;
    int newY = entity->mapY + dy;
    if (newX <= 0 || newY <= 0 || newX >= MAP_WIDTH - 1 || newY >= MAP_HEIGHT - 1) return false;

    // Check if the new position is occupied
    if (entManager->entMap[newY][newX] != NULL) return false;

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
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            Entity* ent = entManager->entMap[i][j];
            if (ent != NULL) disposeEntity(ent);
        }
    }

    heap_delete(entManager->eventQueue);
    free(entManager->eventQueue);
    entManager->eventQueue = NULL;
    free(entManager->entMap);
}


void disposeEntity(Entity* entity) {
    if (entity->soul != NULL && entity->type != PLAYER) free(entity->soul);
    entity->soul = NULL;
    free(entity);
}


