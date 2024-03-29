// This file contains small standalone animation components that can be invoked pretty much everywhere
// They generally do not require coupling or even knowledge of the current context

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <ncurses.h>
#include "utils/mathematics.hpp"

// Window opening animation, every intervalMicros, the window expands by 2 tiles vertically
// The function returns once the animation is complete. The context at the invocation site should now
// be able to draw their own graphics over it normally. (Note that the border is not shared)
// Takes in the size of the window, and the interval between each frame in micros
void verticalExpandAnimation(Rect2D dimensions, int intervalMicros);

// A spiral animation that plays when a battle is initiated. Should be played before drawing the
// main window. The function returns once the animation is complete
void battleTransitionAnimation(int intervalMicros);

#endif
