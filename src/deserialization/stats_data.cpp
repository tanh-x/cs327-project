#include "deserialization/stats_data.h"
#include "utils/string_helpers.hpp"
#include <iostream>

void StatsData::printSelf() const {
    std::string paddedIdentifier = rightPad(
        truncateToEllipses(identifier, MAX_STRING_COLUMN_WIDTH),
        MAX_STRING_COLUMN_WIDTH
    );

    // @formatter:off
    std::cout
        << toStringOrDefault(id)                << COLUMN_SEPARATOR
        << toStringOrDefault(damageClassId)     << COLUMN_SEPARATOR
        << paddedIdentifier                     << COLUMN_SEPARATOR
        << toStringOrDefault(isBattleOnly)      << COLUMN_SEPARATOR
        << toStringOrDefault(gameIndex)         << COLUMN_SEPARATOR
    << std::endl;
    // @formatter:on
}

[[maybe_unused]] StatsData* StatsData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 5) return nullptr;

    // @formatter:off
    int id                  = toIntOrDefault(tokens[0]);
    int damageClassId       = toIntOrDefault(tokens[1]);
    std::string identifier  = tokens[2];
    bool isBattleOnly       = parseToBool(tokens[3]);
    int gameIndex           = toIntOrDefault(tokens[4]);
    // @formatter:on

    return new StatsData(id, damageClassId, identifier, isBattleOnly, gameIndex);
}