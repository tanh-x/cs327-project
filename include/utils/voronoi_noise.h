#include "mathematics.h"

#ifndef VORONOI_NOISE_H
#define VORONOI_NOISE_H

#define RANDOM_VECTOR_PADDING 10.0f
#define VORONOI_POINTS_SEED 216797466

typedef struct {
    int type;
    Vec3 position;
} VoronoiPoint;

void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z);

int sampleVoronoiNoise(Vec3 position, VoronoiPoint points[], int numTypes);

#endif
