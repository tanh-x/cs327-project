#define b 0xDEFCBA9
#define p1 963676709
#define p2 298482817
#define p3 709348097

int globalHashFunction(int globalX, int globalY, int worldSeed) {
    return (((globalX * worldSeed) ^ (globalY ^ b)) * p3 + ((globalX * p1) ^ (globalY * p2))) & 0xffffff;
}

