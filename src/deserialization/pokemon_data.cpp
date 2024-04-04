#include <memory>
#include <iostream>
#include "deserialization/pokemon_data.hpp"
#include "deserialization/deserializers.hpp"
#include "utils/string_helpers.hpp"


void PokemonData::printSelf() const {
    std::string paddedIdentifier = rightPad(truncateToEllipses(identifier, 20), 20);

    // @formatter:off
    std::cout
        << toStringOrDefault(id)                << COLUMN_SEPARATOR
        << paddedIdentifier                     << COLUMN_SEPARATOR
        << toStringOrDefault(speciesId)         << COLUMN_SEPARATOR
        << toStringOrDefault(height)            << COLUMN_SEPARATOR
        << toStringOrDefault(weight)            << COLUMN_SEPARATOR
        << toStringOrDefault(baseExperience)    << COLUMN_SEPARATOR
        << toStringOrDefault(order)             << COLUMN_SEPARATOR
        << (isDefault ? "TRUE" : "FALSE")       << COLUMN_SEPARATOR
        << std::endl;
    // @formatter:on
}

// Deserializes a single line in the CSV file, and instantiates a new PokemonData
PokemonData* PokemonData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 8) return nullptr;

    // @formatter:off
    int id                  = toIntOrDefault(tokens[0]);
    std::string identifier  = tokens[1];
    int speciesId           = toIntOrDefault(tokens[2]);
    int height              = toIntOrDefault(tokens[3]);
    int weight              = toIntOrDefault(tokens[4]);
    int baseExperience      = toIntOrDefault(tokens[5]);
    int order               = toIntOrDefault(tokens[6]);
    bool isDefault          = tokens[7] == "1";
    // @formatter:on

    return new PokemonData(
        id, identifier, speciesId, height, weight, baseExperience, order, isDefault
    );
}

