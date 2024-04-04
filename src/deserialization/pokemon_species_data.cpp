#include "deserialization/pokemon_species_data.hpp"
#include "utils/string_helpers.hpp"
#include <iostream>

void PokemonSpeciesData::printSelf() const {
    std::string paddedIdentifier = rightPad(
        truncateToEllipses(identifier, 10),
        10
    );

    // @formatter:off
    std::cout
        << toStringOrDefault(id)                    << COLUMN_SEPARATOR
        << paddedIdentifier                         << COLUMN_SEPARATOR
        << toStringOrDefault(generationId)          << COLUMN_SEPARATOR
        << toStringOrDefault(evolvesFromSpeciesId)  << COLUMN_SEPARATOR
        << toStringOrDefault(evolutionChainId)      << COLUMN_SEPARATOR
        << toStringOrDefault(colorId)               << COLUMN_SEPARATOR
        << toStringOrDefault(shapeId)               << COLUMN_SEPARATOR
        << toStringOrDefault(habitatId)             << COLUMN_SEPARATOR
        << toStringOrDefault(genderRate)            << COLUMN_SEPARATOR
        << toStringOrDefault(captureRate)           << COLUMN_SEPARATOR
        << toStringOrDefault(baseHappiness)         << COLUMN_SEPARATOR
        << toStringOrDefault(isBaby)                << COLUMN_SEPARATOR
        << toStringOrDefault(hatchCounter)          << COLUMN_SEPARATOR
        << toStringOrDefault(hasGenderDifferences)  << COLUMN_SEPARATOR
        << toStringOrDefault(growthRateId)          << COLUMN_SEPARATOR
        << toStringOrDefault(formsSwitchable)       << COLUMN_SEPARATOR
        << toStringOrDefault(isLegendary)           << COLUMN_SEPARATOR
        << toStringOrDefault(isMythical)            << COLUMN_SEPARATOR
        << toStringOrDefault(order)                 << COLUMN_SEPARATOR
        << toStringOrDefault(conquestOrder)         << COLUMN_SEPARATOR
    << std::endl;
    // @formatter:on
}

PokemonSpeciesData* PokemonSpeciesData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 20) return nullptr;

    // @formatter:off
    int id                      = toIntOrDefault(tokens[0]);
    std::string identifier      = tokens[1];
    int generationId            = toIntOrDefault(tokens[2]);
    int evolvesFromSpeciesId    = toIntOrDefault(tokens[3]);
    int evolutionChainId        = toIntOrDefault(tokens[4]);
    int colorId                 = toIntOrDefault(tokens[5]);
    int shapeId                 = toIntOrDefault(tokens[6]);
    int habitatId               = toIntOrDefault(tokens[7]);
    int genderRate              = toIntOrDefault(tokens[8]);
    int captureRate             = toIntOrDefault(tokens[9]);
    int baseHappiness           = toIntOrDefault(tokens[10]);
    bool isBaby                 = parseToBool(tokens[11]);
    int hatchCounter            = toIntOrDefault(tokens[12]);
    bool hasGenderDifferences   = parseToBool(tokens[13]);
    int growthRateId            = toIntOrDefault(tokens[14]);
    bool formsSwitchable        = parseToBool(tokens[15]);
    bool isLegendary            = parseToBool(tokens[16]);
    bool isMythical             = parseToBool(tokens[17]);
    int order                   = toIntOrDefault(tokens[18]);
    int conquestOrder           = toIntOrDefault(tokens[19]);
    // @formatter:on

    return new PokemonSpeciesData(
        id, identifier, generationId, evolvesFromSpeciesId, evolutionChainId, colorId, shapeId,
        habitatId, genderRate, captureRate, baseHappiness, isBaby, hatchCounter, hasGenderDifferences,
        growthRateId, formsSwitchable, isLegendary, isMythical, order, conquestOrder
    );
}
