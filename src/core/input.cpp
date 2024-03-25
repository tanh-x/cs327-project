#include <ncurses.h>
#include "core/input.hpp"
#include "contexts/context_facade.hpp"

bool globalInputHandler(int key) {
    switch (key) {
        case 'q':
        case 'Q': {
            GAME.quit_game = true;
            break;
        }
        default: return false;
    }
    return true;
}


// Returns whether the input was actually handled or not
bool handlePlayerInput() {
    // Await player input
    int key = getch();

    // Propagate through the global input handler
    if (globalInputHandler(key)) return true;

    // If uncaught, then propagate through the current context input handler
    bool (* handler)(int) = dispatchContextInputHandler(GAME.context->type);
    if (handler(key)) return true;

    // If none of our input handlers caught the key, then return false
    return false;
}