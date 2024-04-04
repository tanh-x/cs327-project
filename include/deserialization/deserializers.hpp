#ifndef DESERIALIZERS_HPP
#define DESERIALIZERS_HPP

#include <string>
#include <climits>
#include <filesystem>

#define EMPTY_INT (INT_MAX)
#define STR_FOR_EMPTY_INT ("-")

std::filesystem::path findCsvFile(const std::string &filename);

void checkCsvParseFlag(const std::string &flag);

int toIntOrDefault(const std::string &s);

std::string toStringOrDefault(int x);

std::string toStringOrDefault(bool x);

bool parseToBool(const std::string &s);

#endif
