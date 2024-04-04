#include "deserialization/pokemon_type_data.h"
#include "utils/string_helpers.hpp"
#include <iostream>

void PokemonTypeData::printSelf() const {
    std::cout
        << toStringOrDefault(pokemonId) << COLUMN_SEPARATOR
        << toStringOrDefault(typeId) << COLUMN_SEPARATOR
        << toStringOrDefault(slot) << COLUMN_SEPARATOR
        << std::endl;
}

[[maybe_unused]] PokemonTypeData* PokemonTypeData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 3) return nullptr;

    int pokemonId = toIntOrDefault(tokens[0]);
    int typeId = toIntOrDefault(tokens[1]);
    int slot = toIntOrDefault(tokens[2]);

    return new PokemonTypeData(pokemonId, typeId, slot);
}