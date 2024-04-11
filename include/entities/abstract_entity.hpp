#ifndef ENTITIES_H
#define ENTITIES_H
// This file holds functionalities related to Entities, which are characters on the map

#include "entities/entity_types/entity_enum.hpp"
#include "pokemon/pokemon.hpp"

typedef struct GameManager GameManager;

// An AbstractEntity is an abstract class that holds data for events on the map
// Thus, the lifespan and existence of the entity object is bound to the map.
//
// Not all entities are tangible entities on the map or exists inside the EntityManager.
// CorporealEntity is a subclass that physically lives on the map and the ..
class AbstractEntity {
protected:
    explicit AbstractEntity(EntityEnum type);

public:
    // When we destruct the entity, make sure to set the player's pointer to this entity as null
    ~AbstractEntity();

    // The type of entity
    EntityEnum type;

    // A randomly generated name for the entity
    const char* name;

    // Whether the player can battle_opponent this NPC. Entities that can't be fought or has already been defeated will
    // have this value as false
    bool activeBattle;

    // A vector of pokemon that this entity has. Every NPC and the player must have
    // some Pokemon. Wild Pokemon are temporarily given a proxy entity for the purpose
    // of the battle.
    std::vector<Pokemon> pokemonInventory {};
};


#endif
