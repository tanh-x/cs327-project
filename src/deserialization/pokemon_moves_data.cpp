#include <iostream>
#include "deserialization/pokemon_moves_data.hpp"
#include "deserialization/deserializers.hpp"
#include "utils/string_helpers.hpp"

void PokemonMovesData::printSelf() const {
    // @formatter:off
    std::cout
        << toStringOrDefault(pokemonId)             << COLUMN_SEPARATOR
        << toStringOrDefault(versionGroupId)        << COLUMN_SEPARATOR
        << toStringOrDefault(moveId)                << COLUMN_SEPARATOR
        << toStringOrDefault(pokemonMoveMethodId)   << COLUMN_SEPARATOR
        << toStringOrDefault(level)                 << COLUMN_SEPARATOR
        << toStringOrDefault(order)                 << COLUMN_SEPARATOR
        << std::endl;
    // @formatter:on
}

PokemonMovesData* PokemonMovesData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() < 5) return nullptr;

    // @formatter:off
    int pokemonId               = toIntOrDefault(tokens[0]);
    int versionGroupId          = toIntOrDefault(tokens[1]);
    int moveId                  = toIntOrDefault(tokens[2]);
    int pokemonMoveMethodId     = toIntOrDefault(tokens[3]);
    int level                   = toIntOrDefault(tokens[4]);
    int order                   = toIntOrDefault(tokens[5]);
    // @formatter:on

    return new PokemonMovesData(pokemonId, versionGroupId, moveId, pokemonMoveMethodId, level, order);
}
