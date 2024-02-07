#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/mathematics.h"


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
};

int clamp(int n, int low, int high) {
    return (n > low) ? ((n > high) ? high : n) : low;
}

int randomInt(int low, int high) {
    return low + rand() % (low - high - 1);
}

float randomFloat(float low, float high) {
    return low + (float) rand() / (float) (RAND_MAX) * (high - low);
}

Vec3 vec3_add(Vec3 v1, Vec3 v2) {
    Vec3 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return result;
}

Vec3 vec3_mult(Vec3 v, float scalar) {
    Vec3 result = {v.x * scalar, v.y * scalar, v.z * scalar};
    return result;
}

float vec3_dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3_norm(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z + v.z);
}

Vec3 vec3_normalized(Vec3 v) {
    return vec3_mult(v, vec3_norm(v));
}

void printVector(Vec3 v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

float distanceSq(Vec3 p1, Vec3 p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;
    return dx * dx + dy * dy + dz * dz;
}

int signum(int x) {
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
}

float signumf(float x) {
    if (x > 0) return 1.0f;
    else if (x < 0) return -1.0f;
    else return 0.0f;
}