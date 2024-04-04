#include <iostream>
#include "deserialization/type_name_data.hpp"
#include "utils/string_helpers.hpp"

#define ENGLISH_LANGUAGE_ID 9

void TypeNameData::printSelf() const {
    std::cout
        << toStringOrDefault(id) << COLUMN_SEPARATOR
        << rightPad(name, MAX_STRING_COLUMN_WIDTH) << COLUMN_SEPARATOR
        << std::endl;
}


[[maybe_unused]] TypeNameData* TypeNameData::deserialize(const std::string &line) {
    std::vector<std::string> tokens = splitString(line, ',');

    if (tokens.size() != 3) return nullptr;

    // Only get English text entries
    if (toIntOrDefault(tokens[1]) != ENGLISH_LANGUAGE_ID) return nullptr;

    int id = toIntOrDefault(tokens[0]);
    std::string name = tokens[2];

    return new TypeNameData(id, name);
}