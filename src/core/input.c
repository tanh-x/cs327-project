#include <ncurses.h>
#include "core/input.h"
#include "contexts/context.h"

bool globalInputHandler(GameManager* game, __attribute__((unused)) GameOptions* options, int key) {
    switch (key) {
        case 'q':
        case 'Q': {
            game->quit_game = true;
            break;
        }
        default: return false;
    }
    return true;
}


// Returns whether the input was actually handled or not
bool handlePlayerInput(GameManager* game, GameOptions* options) {
    // Await player input
    int key = getch();

    // Propagate through the global input handler
    if (globalInputHandler(game, options, key)) return true;

    // If uncaught, then propagate through the current context input handler
    bool (* handler)(GameManager*, GameOptions*, int) = dispatchContextInputHandler(game->context);
    if (handler(game, options, key)) return true;

    // If none of our input handlers caught the key, then return false
    return false;
}