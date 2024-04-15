#ifndef CONSTANTS_H
#define CONSTANTS_H
// Constants that are very frequently used throughout every part of the project

// Number of retries to in repeated random rolls
#define MAX_ITERATIONS_SMALL 32
#define MAX_ITERATIONS 8192

// Dimensions of the window and map
// WIDTH - 2 must be divisible by 3
//#define WINDOW_WIDTH 80
//#define WINDOW_HEIGHT 24
//#define MAP_WIDTH 80
//#define MAP_HEIGHT 21
#define WINDOW_WIDTH 116
#define WINDOW_HEIGHT 36
#define MAP_WIDTH 116
#define MAP_HEIGHT 33

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

// Miscellaneous constants
#define ESCAPE_KEY 27

#define STD_SLOW_FRAME_DELAY 200
#define INTERVAL_60FPS_MICROS 16667
#define INTERVAL_30FPS_MICROS 33333

#endif
