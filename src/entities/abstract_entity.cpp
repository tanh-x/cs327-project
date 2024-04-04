// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.hpp"
#include "entities/abstract_entity.hpp"
#include "core/player.hpp"
#include "entities/entity_types/pacer.hpp"
#include "entities/entity_manager.hpp"
#include "entities/entity_types/hiker.hpp"
#include "entities/entity_types/rival.hpp"
#include "entities/entity_types/wanderer.hpp"
#include "entities/entity_types/sentry.hpp"
#include "entities/entity_types/explorer.hpp"

#define DEFAULT_IDLE_COST 8

const char* trainerNames[] = TRAINER_NAMES;

AbstractEntity::AbstractEntity(EntityEnum type, int x, int y) {
    this->type = type;
    this->mapX = x;
    this->mapY = y;


    if (type == EntityEnum::PLAYER) {
        this->name = "Player";
        this->activeBattle = false;
    } else {
        // If they're an NPC, give them a random name
        this->name = trainerNames[randomInt(0, NUM_TRAINER_NAMES - 1)];
        this->activeBattle = true;
    }
}

// When we destruct the entity, make sure to set the player's pointer to this entity as null
AbstractEntity::~AbstractEntity() {
    if (this->type == EntityEnum::PLAYER) {
        Player* player = GAME.player;
        if (player != nullptr) player->currentEntity = nullptr;
    }
}

// TODO: Entity moveBy currently coupled to the current EntityManager. Make sure to check whether the entity actually lies inside the current ent manager
// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
// If it was unsuccessful (false return), it will be one of the three following scenarios:
// - The EntityManager was falsy/null
// - The specified position was out of bounds
// - The tile is already occupied by another entity
bool AbstractEntity::moveBy(int dx, int dy) {
    EntityManager* entManager = GAME.currentEntManager;
    if (!entManager) return false;

    int newX = this->mapX + dx;
    int newY = this->mapY + dy;

    // Boundary check for new position
    if (!isInsideMapBounds(newX, newY)) return false;

    // Check if the new position is occupied
    if (entManager->entMap[newY][newX] != nullptr) return false;

    // If we got here, it will be a valid move, nothing should've been mutated before this point

    // Remove the entity from the previous spot in the entity map
    entManager->entMap[this->mapY][this->mapX] = nullptr;

    // Update the positions
    this->mapX = newX;
    this->mapY = newY;

    // Add it back into the new position in the entity map
    entManager->entMap[newY][newX] = this;

    // Update player position if entity is a player
    if (this->type == EntityEnum::PLAYER) {
        GAME.player->mapX = newX;
        GAME.player->mapY = newY;
    }

    return true;
}

// @formatter:off
AbstractEntity* AbstractEntity::spawnNpc(EntityEnum type, int mapX, int mapY) {
    switch (type) {
        case EntityEnum::HIKER:     return new Hiker(mapX, mapY);
        case EntityEnum::RIVAL:     return new Rival(mapX, mapY);
        case EntityEnum::PACER:     return new Pacer(mapX, mapY);
        case EntityEnum::WANDERER:  return new Wanderer(mapX, mapY);
        case EntityEnum::SENTRY:    return new Sentry(mapX, mapY);
        case EntityEnum::EXPLORER:  return new Explorer(mapX, mapY);
        default: return nullptr;
    }
}
// @formatter:on

// Creates a new event according to the entity's AI
Event* AbstractEntity::constructEventOnTurn() {
    // Initialize new event object
    auto* event = static_cast<Event*>(malloc(sizeof(Event)));
    event->type = MOVEMENT;
    event->dx = 0;
    event->dy = 0;
    event->actor = this;

    bool success;
    // Check if the entity wants to fight the player or not
    if (activeBattle) {
        // If so, then move according to the entity's AI function
        success = moveAI(event);
    } else {
        // If the entity doesn't want to fight the player, don't pathfind towards them
        success = false;
    }

    // If we didn't succeed, that mean the AI couldn't find a valid move for this turn, or activeBattle is false
    // Just wait for a tiny bit if this is the case.
    if (!success) {
        disposeEvent(event);
        return constructIdleEvent(this, DEFAULT_IDLE_COST);
    }

    return event;
}
