#ifndef PROJECT_MATHEMATICS_H
#define PROJECT_MATHEMATICS_H

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec3_add(Vec3 v1, Vec3 v2);

Vec3 vec3_mult(Vec3 v, float scalar);

float vec3_dot(Vec3 v1, Vec3 v2);

float vec3_norm(Vec3 v);

Vec3 vec3_normalized(Vec3 v);

#endif
