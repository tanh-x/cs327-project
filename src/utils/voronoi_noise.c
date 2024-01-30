#include <stdlib.h>
#include <float.h>
#include "../../include/utils/voronoi_noise.h"
#include "../../include/world/mapbuilder.h"

#define RANDOM_VECTOR_PADDING 6.0f

void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z, int seed) {
    srand(seed);

    for (int t = 0; t < numTypes; t++) {
        for (int i = 0; i < numPointsPerType; i++) {
            Vec3 position = {
                randomFloat(-RANDOM_VECTOR_PADDING, MAP_WIDTH + RANDOM_VECTOR_PADDING),
                randomFloat(-RANDOM_VECTOR_PADDING, MAP_HEIGHT + RANDOM_VECTOR_PADDING) * VERTICAL_SCALING_FACTOR,
                randomFloat(z - RANDOM_VECTOR_PADDING, z + RANDOM_VECTOR_PADDING)
            };
            points[i + t * numPointsPerType].position = position;
            points[i + t * numPointsPerType].type = t;
        }
    }
}

int sampleVoronoiNoise( VoronoiPoint points[], int numPoints, Vec3 position, float lacunarity) {
    float minDist = FLT_MAX;
    int result = 0;

    for (int i = 0; i < numPoints; i++) {
        float distance = distanceSq(position, points[i].position) + randomFloat(-lacunarity, lacunarity);
        if (distance < minDist) {
            minDist = distance;
            result = points[i].type;
        }
    }
    return result;
}