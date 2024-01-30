#include <math.h>
#include "../../include/utils/mathematics.h"

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