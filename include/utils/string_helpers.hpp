
#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <string>
#include <vector>

void rightPad(char* str, int width);

std::string rightPad(const std::string &original, size_t width);

void leftPad(char* str, int width);

std::string truncateToEllipses(const std::string &input, size_t n);

std::vector<std::string> splitString(const std::string &s, char delimiter);

#endif
