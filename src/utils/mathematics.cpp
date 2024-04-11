#include "utils/mathematics.hpp"
#include <cstdlib>
#include <cmath>

int positiveMod(int x, int y) {
    return ((x % y) + y) % y;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

float minf(float a, float b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

float maxf(float a, float b) {
    return (a > b) ? a : b;
}

int clamp(int n, int low, int high) {
    return (n > low) ? ((n > high) ? high : n) : low;
}

int randomInt(int low, int high) {
    return low + rand() % (low - high - 1);
}

float randomFloat(float low, float high) {
    return low + (float) rand() / (float) (RAND_MAX) * (high - low);
}

[[maybe_unused]] Float3D vec3_add(Float3D v1, Float3D v2) {
    Float3D result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return result;
}

[[maybe_unused]] Float3D vec3_mult(Float3D v, float scalar) {
    Float3D result = {v.x * scalar, v.y * scalar, v.z * scalar};
    return result;
}

[[maybe_unused]] float vec3_dot(Float3D v1, Float3D v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

[[maybe_unused]] float vec3_norm(Float3D v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z + v.z);
}

[[maybe_unused]] Float3D vec3_normalized(Float3D v) {
    return vec3_mult(v, vec3_norm(v));
}

float distanceSq(Float3D p1, Float3D p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;
    return dx * dx + dy * dy + dz * dz;
}

int signum(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

float signumf(float x) {
    if (x > 0) return 1.0f;
    if (x < 0) return -1.0f;
    return 0.0f;
}

int manhattanDist(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

float getBaseNumTrainers(float wilderness) {
    return (-8.86f * wilderness + 5.125f) / (4.93f * wilderness - 0.05f);
}