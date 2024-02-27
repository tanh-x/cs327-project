#include "core/game.h"
#include "entity/entities.h"
#include "entity/event.h"
#include "core/player.h"
#include "entity/npc/pacer.h"
#include "entity/npc/wanderer.h"

void* getCharacterSoul(Entity* ent, GameManager* game) {
    switch (ent->type) {
        case PLAYER: return game->player;
        case PACER: return constructPacerSoul();
        case WANDERER: return constructWandererSoul(game->world->current->tileset[ent->mapY][ent->mapX].type);
        default: return NULL;
    }
}

Entity* spawnEntity(GameManager* game, EntityType type, int x, int y) {
    EntityManager* entManager = game->entManager;
    if (entManager->entMap[y][x] != NULL) return NULL;

    Entity* entity = malloc(sizeof(Entity));
    entity->type = type;
    entity->mapX = x;
    entity->mapY = y;
    entManager->entMap[y][x] = entity;

    entity->soul = getCharacterSoul(entity, game);

    return entity;
}

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

void disposeEntityManager(EntityManager* entManager) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            Entity* ent = entManager->entMap[i][j];
            if (ent != NULL) free(ent);
        }
    }

    heap_delete(entManager->eventQueue);
    free(entManager->entMap);
}

