#include "mathematics.h"

#ifndef VORONOI_NOISE_H
#define VORONOI_NOISE_H

#define RANDOM_VECTOR_PADDING 6.0f
#define VERTICAL_SCALING_FACTOR 2.5268f
#define VORONOI_POINTS_SEED 216797466

typedef struct {
    int type;
    Vec3 position;
} VoronoiPoint;

void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z, int seed);

int sampleVoronoiNoise(Vec3 position, VoronoiPoint points[], int numTypes);

#endif
