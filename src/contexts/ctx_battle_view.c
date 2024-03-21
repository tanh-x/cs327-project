#include <ncurses.h>
#include "contexts/ctx_battle_view.h"
#include "core/game_manager.h"
#include "graphics/artist.h"

void enterPlaceholderBattle(Entity* opponent) {
    // Define dimensions of the new window
    Rect2D windowDimensions;
    windowDimensions.width = WINDOW_WIDTH - 4;
    windowDimensions.height = WINDOW_HEIGHT - 4;

    // Find the center of the parent window
    windowDimensions.x = (WINDOW_WIDTH - windowDimensions.width) / 2;
    windowDimensions.y = (WINDOW_HEIGHT - windowDimensions.height) / 2;


    // Construct and switch to it
    Context* context = constructChildWindowContext(BATTLE_CONTEXT, windowDimensions);
    WINDOW* window = context->window;

    // Add a placeholder title
    mvwprintw(window, 1, 1, "PLACEHOLDER BATTLE INTERFACE (type %c) [@ %p]", entityToChar(opponent), &opponent->soul);

    // We're done with initialization
    wrefresh(window);

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == ESCAPE_KEY || ch == '`' || ch == '~') break;
    }

    // If we got here, ESC has been pressed
    opponent->activeBattle = false;
    GAME.context = context->parent;
    returnToParentContext(context);
}
