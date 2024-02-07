#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#define proba() randomFloat(0.0f, 1.0f)

int positiveMod(int x, int y);

int min(int a, int b);

float minf(float a, float b);

int max(int a, int b);

float maxf(float a, float b);

int clamp(int n, int low, int high);

float randomFloat(float low, float high);

int randomInt(int low, int high);

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

void printVector(Vec3 v);

float distanceSq(Vec3 p1, Vec3 p2);

int signum(int x);

float signumf(float x);

#endif
