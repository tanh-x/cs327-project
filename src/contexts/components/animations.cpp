#include <unistd.h>
#include "contexts/components/animations.hpp"
#include "contexts/components/keyframes.hpp"

#define EIGHT_SPACES "        "


void expandWindowVertical(Rect2D dimensions, int intervalMicros) {
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


int battleTransitionKeyframes[NUM_BATTLE_TRANSITION_KEYFRAMES] = BATTLE_TRANSITION_KEYFRAMES;

void battleTransition(int intervalMicros) {
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