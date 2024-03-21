#include <string.h>
#include "utils/string_helpers.h"

void rightPad(char* str, int width) {
    int len = strlen(str);  //NOLINT
    if (len < width) {
        memset(str + len, ' ', width - len);
        str[width] = '\0';
    }
}