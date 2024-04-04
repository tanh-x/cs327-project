#include <cstring>
#include <sstream>
#include "utils/string_helpers.hpp"

void rightPad(char* str, int width) {
    int len = strlen(str);  // NOLINT
    if (len < width) {
        memset(str + len, ' ', width - len);
        str[width] = '\0';
    }
}

std::string rightPad(const std::string &original, size_t width) {
    std::string str = original;
    size_t len = str.length();
    if (len < width) str.append(width - len, ' ');
    return str;
}

void leftPad(char* str, int width) {
    int len = strlen(str); // NOLINT
    if (len >= width) return;

    memmove(str + width - len, str, len + 1);
    memset(str, ' ', width - len);
    str[width] = '\0';
}


std::string truncateToEllipses(const std::string &input, size_t n) {
    if (input.length() <= n || n <= 3) return input;
    return input.substr(0, n - 3) + "...";
}


std::vector<std::string> splitString(const std::string &s, char delimiter) {
    std::vector<std::string> result;
    auto stream = std::stringstream(s);
    std::string item;

    while (std::getline(stream, item, delimiter)) result.push_back(item);

    return result;
}