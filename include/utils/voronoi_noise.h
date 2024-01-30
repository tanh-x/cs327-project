#include "mathematics.h"

#ifndef VORONOI_NOISE_H
#define VORONOI_NOISE_H

#define VERTICAL_SCALING_FACTOR 3.2268f

typedef struct {
    int type;
    Vec3 position;
} VoronoiPoint;

void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z, int seed);

int sampleVoronoiNoise(VoronoiPoint points[], int numPoints, Vec3 position, float lacunarity);

#endif
