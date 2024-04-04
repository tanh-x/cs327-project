#include <vector>
#include <iostream>
#include "deserialization/move_data.hpp"
#include "utils/string_helpers.hpp"
#include "deserialization/deserializers.hpp"


void MoveData::printSelf() const {
    std::string paddedIdentifier = rightPad(truncateToEllipses(identifier, 20), 20);

    // @formatter:off
    std::cout
        << toStringOrDefault(id)                        << COLUMN_SEPARATOR
        << paddedIdentifier                             << COLUMN_SEPARATOR
        << toStringOrDefault(generationId)              << COLUMN_SEPARATOR
        << toStringOrDefault(typeId)                    << COLUMN_SEPARATOR
        << toStringOrDefault(power)                     << COLUMN_SEPARATOR
        << toStringOrDefault(pp)                        << COLUMN_SEPARATOR
        << toStringOrDefault(accuracy)                  << COLUMN_SEPARATOR
        << toStringOrDefault(priority)                  << COLUMN_SEPARATOR
        << toStringOrDefault(targetId)                  << COLUMN_SEPARATOR
        << toStringOrDefault(damageClassId)             << COLUMN_SEPARATOR
        << toStringOrDefault(effectId)                  << COLUMN_SEPARATOR
        << toStringOrDefault(effectChance)              << COLUMN_SEPARATOR
        << toStringOrDefault(contestTypeId)             << COLUMN_SEPARATOR
        << toStringOrDefault(contestEffectId)           << COLUMN_SEPARATOR
        << toStringOrDefault(superContestEffectId)      << COLUMN_SEPARATOR
        << std::endl;
    // @formatter:on
}


MoveData* MoveData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 15) return nullptr;

    // @formatter:off
    int id                      = toIntOrDefault(tokens[0]);
    std::string identifier      = tokens[1];
    int generationId            = toIntOrDefault(tokens[2]);
    int typeId                  = toIntOrDefault(tokens[3]);
    int power                   = toIntOrDefault(tokens[4]);
    int pp                      = toIntOrDefault(tokens[5]);
    int accuracy                = toIntOrDefault(tokens[6]);
    int priority                = toIntOrDefault(tokens[7]);
    int targetId                = toIntOrDefault(tokens[8]);
    int damageClassId           = toIntOrDefault(tokens[9]);
    int effectId                = toIntOrDefault(tokens[10]);
    int effectChance            = toIntOrDefault(tokens[11]);
    int contestTypeId           = toIntOrDefault(tokens[12]);
    int contestEffectId         = toIntOrDefault(tokens[13]);
    int superContestEffectId    = toIntOrDefault(tokens[14]);
    // @formatter:on

    return new MoveData(
        id, identifier, generationId, typeId, power, pp, accuracy, priority, targetId,
        damageClassId, effectId, effectChance, contestTypeId, contestEffectId, superContestEffectId
    );
}