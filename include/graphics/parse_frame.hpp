#ifndef PARSE_FRAME_H
#define PARSE_FRAME_H

#define SPRITE_ATLAS_WIDTH 277
#define SPRITE_ATLAS_HEIGHT 216

int **parseFrame(char *filename, int width, int height);

Int2D getAtlasCoordinate(int pokemonId);

extern int** spriteAtlas;

#endif