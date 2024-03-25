// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.hpp"
#include "entity/entities.hpp"
#include "entity/event.hpp"
#include "core/player.hpp"
#include "entity/npc/pacer.hpp"
#include "entity/entity_manager.hpp"


const char* trainerNames[] = TRAINER_NAMES;


Entity::Entity(EntityType type, int x, int y) {
    this->type = type;
    this->mapX = x;
    this->mapY = y;

    this->activeBattle = true;

    // If they're an NPC, give them a random name
    if (type != PLAYER) this->name = "Player";
    else this->name = trainerNames[randomInt(0, NUM_TRAINER_NAMES - 1)];

    // Add it to the entity list, and then return it
}

Entity::~Entity() {
    if (this->type == PLAYER) {
        Player* player = GAME.player;
        if (player != nullptr) player->currentEntity = nullptr;
    }
}

// Moves the entity to the new location, doing all the necessary checks to make sure it's a valid move.
// Returns a boolean indicating whether it was successful. If it was not, no side effects will have been made.
// If it was unsuccessful (false return), it will be one of the three following scenarios:
// - The EntityManager was falsy/null
// - The specified position was out of bounds
// - The tile is already occupied by another entity
bool Entity::moveBy(int dx, int dy) {
    EntityManager* entManager = GAME.entManager;
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
    if (this->type == PLAYER) {
        GAME.player->mapX = newX;
        GAME.player->mapY = newY;
    }

    return true;
}