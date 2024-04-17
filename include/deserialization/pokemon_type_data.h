#ifndef POKEMON_TYPES_DATA_H
#define POKEMON_TYPES_DATA_H

#include <string>
#include "abstract_deserializable.hpp"

#define POKEMON_TYPE_DATA_CSV_NAME "pokemon_types.csv"

class PokemonTypeRelation : public AbstractDeserializable {
public:
    int pokemonId;
    int typeId;
    int slot;

    PokemonTypeRelation(
        int pokemonId,
        int typeId,
        int slot
    ) : AbstractDeserializable(),
        pokemonId(pokemonId),
        typeId(typeId),
        slot(slot) {}

    void printSelf() const override;

    [[maybe_unused]] static auto deserialize(const std::string &line) -> PokemonTypeRelation*;
};

#endif
