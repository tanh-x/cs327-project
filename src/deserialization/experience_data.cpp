#include <iostream>
#include "deserialization/experience_data.hpp"
#include "utils/string_helpers.hpp"

void ExperienceData::printSelf() const {
    std::cout
        << toStringOrDefault(growthRateId) << COLUMN_SEPARATOR
        << toStringOrDefault(level) << COLUMN_SEPARATOR
        << rightPad(toStringOrDefault(experience), 8) << COLUMN_SEPARATOR
        << std::endl;
}

[[maybe_unused]] ExperienceData* ExperienceData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 3) return nullptr;

    int growthRateId = toIntOrDefault(tokens[0]);
    int level = toIntOrDefault(tokens[1]);
    int experience = toIntOrDefault(tokens[2]);

    return new ExperienceData(growthRateId, level, experience);
}

