#ifndef CONSTANTS_H
#define CONSTANTS_H
// Constants that are very frequently used throughout every part of the project

// Number of retries to in repeated random rolls
#define MAX_ITERATIONS_SMALL 32
#define MAX_ITERATIONS 8192

// Dimensions of the map
#define MAP_WIDTH 80
#define MAP_HEIGHT 21

// Dimensions of the world. Recommended to be an odd number
#define WORLD_WIDTH 401
#define WORLD_HEIGHT 401
// Span must be (dimension - 1)/2
#define WORLD_X_SPAN 200
#define WORLD_Y_SPAN 200

// Padding denotes the exclusion zone on the edge of maps, where certain parts of the map generation algorithm
// as well as AI actions are blocked or otherwise changed.
#define PADDING 3
#define GATE_PADDING 7

// Used in pathfinding and distance fields
#define UNCROSSABLE 8001001
#define UNVISITED (-1)

#endif
