#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "context/abstract_context.hpp"

void windowTitle(AbstractContext* context, const char* title, int x);

void horizontalSeparator(AbstractContext* context, int x, int y, int width);

void verticalSeparator(AbstractContext* context, int x, int y, int height);

void spaces(AbstractContext* context, int x, int y, int width);

void sequentialColoredBar(
    AbstractContext* context, int x, int y,
    int width, float value,
    int cmapPaletteOffset, int numPaletteColors
);

#endif
