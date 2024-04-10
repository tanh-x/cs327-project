#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#define proba() randomFloat(0.0f, 1.0f)

typedef enum __attribute__ ((__packed__)) {
    NORTH, SOUTH, WEST, EAST
} CardinalDir;

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
} Float3D;

Float3D vec3_add(Float3D v1, Float3D v2);

Float3D vec3_mult(Float3D v, float scalar);

float vec3_dot(Float3D v1, Float3D v2);

float vec3_norm(Float3D v);

[[maybe_unused]] Float3D vec3_normalized(Float3D v);

float distanceSq(Float3D p1, Float3D p2);

int signum(int x);

float signumf(float x);

int manhattanDist(int x1, int y1, int x2, int y2);

typedef struct {
    int x;
    int y;
} Int2D;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect2D;

#endif
