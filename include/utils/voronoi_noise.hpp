#ifndef VORONOI_NOISE_H
#define VORONOI_NOISE_H

#include "mathematics.hpp"

#define VERTICAL_SCALING_FACTOR 3.2268f

typedef struct {
    int type;
    Float3D position;
} VoronoiPoint;

void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z, int seed);

int sampleVoronoiNoise(VoronoiPoint points[], int numPoints, Float3D position, float lacunarity);

#endif
