#include "entities/entity_types/pokemon_vessel.hpp"

#include <utility>

PokemonVessel::PokemonVessel(const std::shared_ptr<Pokemon> &pokemon) : AbstractEntity(EntityEnum::POKEMON_VESSEL) {
    name = "A wild pokemon";
    pokemonInventory = {pokemon};
}
