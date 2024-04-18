#ifndef MISC_H
#define MISC_H


#include <vector>
#include <algorithm>

template <typename T>
bool contains(const std::vector<T>& vec, const T& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

#endif
