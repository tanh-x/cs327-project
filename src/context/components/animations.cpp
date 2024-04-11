// This file contains small standalone animation components that can be invoked pretty much everywhere
// They generally do not require coupling or even knowledge of the current context

#include <unistd.h>
#include "context/components/animations.hpp"
#include "context/components/keyframes.hpp"
#include "graphics/renderer.hpp"

// Window opening animation, every intervalMicros, the window expands by 2 tiles vertically
// The function returns once the animation is complete. The context at the invocation site should now
// be able to draw their own graphics over it normally. (Note that the border is not shared)
// Takes in the size of the window, and the interval between each frame in micros
void verticalExpandAnimation(Rect2D dimensions, int intervalMicros) {
    int half = dimensions.y + dimensions.height / 2;

    for (int span = 0; half - span >= dimensions.y; span++) {
        WINDOW* tempBox = newwin(
            min(1 + 2 * span, dimensions.height),
            dimensions.width,
            half - span,
            dimensions.x
        );
        box(tempBox, 0, 0);
        wrefresh(tempBox);
        usleep(intervalMicros);
        delwin(tempBox);
    }
}

// Like verticalExpandAnimation(), but horizontal
void horizontalExpandAnimation(Rect2D dimensions, int intervalMicros) {
    int center = dimensions.x + dimensions.width / 2;

    for (int span = 0; center - span >= dimensions.x; span += 3) {
        WINDOW* tempBox = newwin(
            dimensions.height,
            min(1 + 2 * span, dimensions.width),
            dimensions.y,
            center - span
        );
        box(tempBox, 0, 0);
        wrefresh(tempBox);
        usleep(intervalMicros);
        delwin(tempBox);
    }
}

// Keyframes for the battle_opponent transition, which are sequential (x, y) positions of blacked out chunks
int battleTransitionKeyframes[NUM_BATTLE_TRANSITION_KEYFRAMES] = BATTLE_TRANSITION_KEYFRAMES;

// A spiral animation that plays when a battle_opponent is initiated. Should be played before drawing the
// main window. The function returns once the animation is complete
void battleTransitionAnimation(int intervalMicros) {
    for (int i = 0; i < NUM_BATTLE_TRANSITION_KEYFRAMES; i += 2) {
        int top = battleTransitionKeyframes[i];
        int left = battleTransitionKeyframes[i + 1];
        for (int dy = 0; dy < 4; dy++) {
            for (int dx = 0; dx < 8; dx++) {
                mvaddch(top + dy, left + dx, ' ');
            }
        }
        refresh();
        usleep(intervalMicros);
    }
}