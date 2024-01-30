#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "../../include/utils/voronoi_noise.h"
#include "../../include/world/mapbuilder.h"


void initializeVoronoiPoints(VoronoiPoint points[], int numPointsPerType, int numTypes, float z) {
    srand(VORONOI_POINTS_SEED + (int) floorf(z));

    for (int t = 0; t < numTypes; t++) {
        for (int i = 0; i < numPointsPerType; i++) {
            Vec3 position = {
                randomFloat(-RANDOM_VECTOR_PADDING, MAP_WIDTH + RANDOM_VECTOR_PADDING),
                randomFloat(-RANDOM_VECTOR_PADDING, MAP_HEIGHT + RANDOM_VECTOR_PADDING),
                randomFloat(z - RANDOM_VECTOR_PADDING, z + RANDOM_VECTOR_PADDING)
            };
            points[i + t * numPointsPerType].position = position;
            points[i + t * numPointsPerType].type = t;
        }
    }
}

int sampleVoronoiNoise(Vec3 position, VoronoiPoint points[], int numPoints) {
    float minDist = FLT_MAX;
    int result = 0;

    for (int i = 0; i < numPoints; i++) {
        float distance = sqrtf(distanceSq(position, points[i].position));
        if (distance < minDist) {
            minDist = distance;
            result = points[i].type;
        }
    }
    return result;
}