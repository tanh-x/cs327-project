#include <string.h>
#include "utils/string_helpers.hpp"

void rightPad(char* str, int width) {
    int len = strlen(str);  // NOLINT
    if (len < width) {
        memset(str + len, ' ', width - len);
        str[width] = '\0';
    }
}


void leftPad(char* str, int width) {
    int len = strlen(str); // NOLINT
    if (len >= width) return;

    memmove(str + width - len, str, len + 1);
    memset(str, ' ', width - len);
    str[width] = '\0';
}