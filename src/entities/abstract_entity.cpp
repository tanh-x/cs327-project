// This file holds functionalities related to Entities, which are characters on the map
#include "core/game.hpp"
#include "entities/abstract_entity.hpp"
#include "core/player.hpp"
#include "core/constants/text.hpp"

const char* trainerNames[] = TRAINER_NAMES;

AbstractEntity::AbstractEntity(EntityEnum type) {
    this->type = type;

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
