#include <iostream>
#include "deserialization/pokemon_stats_data.hpp"
#include "utils/string_helpers.hpp"
#include "deserialization/deserializers.hpp"

void PokemonStatsData::printSelf() const {
    std::cout
        << toStringOrDefault(pokemonId) << COLUMN_SEPARATOR
        << toStringOrDefault(statId) << COLUMN_SEPARATOR
        << toStringOrDefault(baseStat) << COLUMN_SEPARATOR
        << toStringOrDefault(effort) << COLUMN_SEPARATOR
        << std::endl;
}

[[maybe_unused]] PokemonStatsData* PokemonStatsData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 4) return nullptr;

    int pokemonId = toIntOrDefault(tokens[0]);
    int statId = toIntOrDefault(tokens[1]);
    int baseStat = toIntOrDefault(tokens[2]);
    int effort = toIntOrDefault(tokens[3]);

    return new PokemonStatsData(pokemonId, statId, baseStat, effort);
}