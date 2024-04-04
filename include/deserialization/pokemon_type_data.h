#ifndef POKEMON_TYPES_DATA_H
#define POKEMON_TYPES_DATA_H

#include <string>
#include "deserializers.hpp"

#define POKEMON_TYPE_DATA_CSV_NAME "pokemon_types.csv"

class PokemonTypeData : public AbstractDeserializable {
public:
    int pokemonId;
    int typeId;
    int slot;

    PokemonTypeData(
        int pokemonId,
        int typeId,
        int slot
    ) : AbstractDeserializable(),
        pokemonId(pokemonId),
        typeId(typeId),
        slot(slot) {}

    void printSelf() const override;

    [[maybe_unused]] static PokemonTypeData* deserialize(const std::string &line);
};

#endif
