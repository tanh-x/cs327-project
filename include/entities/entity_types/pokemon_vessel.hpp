#ifndef POKEMON_VESSEL_H
#define POKEMON_VESSEL_H


#include "entities/abstract_entity.hpp"

class PokemonVessel : public AbstractEntity {
public:
    PokemonVessel(const std::shared_ptr<Pokemon> &pokemon);
};

#endif