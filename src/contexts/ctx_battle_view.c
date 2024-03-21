#include <ncurses.h>
#include <unistd.h>
#include "contexts/ctx_battle_view.h"
#include "core/game_manager.h"
#include "graphics/artist.h"
#include "contexts/components/animations.h"

void enterPlaceholderBattle(Entity* opponent) {
    // Define dimensions of the new window
    Rect2D windowDimensions;
    windowDimensions.width = WINDOW_WIDTH;
    windowDimensions.height = WINDOW_HEIGHT;

    // Find the center of the parent window
    windowDimensions.x = (WINDOW_WIDTH - windowDimensions.width) / 2;
    windowDimensions.y = (WINDOW_HEIGHT - windowDimensions.height) / 2;

    battleTransition(INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    Context* context = constructChildWindowContext(BATTLE_CONTEXT, windowDimensions);
    WINDOW* window = context->window;
    // Add a placeholder title
    mvwprintw(window, 1, 2, "PLACEHOLDER BATTLE INTERFACE");
    // We're done with the main window

    // TIme to build the dialog window
    Rect2D dialogDimensions;
    dialogDimensions.x = 1;
    dialogDimensions.y = WINDOW_HEIGHT - DIALOG_WINDOW_HEIGHT;
    dialogDimensions.width = WINDOW_WIDTH - 2;
    dialogDimensions.height = DIALOG_WINDOW_HEIGHT;
    wrefresh(window);
    usleep(STD_SLOW_FRAME_DELAY);

    // Start with an animation
    expandWindowVertical(dialogDimensions, INTERVAL_30FPS_MICROS);

    // Then build the actual dialog window
    WINDOW* dialogWindow = constructWindow(dialogDimensions);

    // Print out text to the dialog window
    mvwprintw(
        dialogWindow, 1, 1, "%s wants to fight!",
        opponent->name
    );
    mvwprintw(
        dialogWindow, DIALOG_WINDOW_HEIGHT - 2, 1, "<Opponent: \"%s\" (type %c) [@ %p]>",
        opponent->name, entityToChar(opponent), &opponent->soul
    );

    // We're done with building the window
    wrefresh(dialogWindow);

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == ESCAPE_KEY || ch == '`' || ch == '~') break;
    }

    // If we got here, ESC has been pressed
    clear();
    opponent->activeBattle = false;
    GAME.context = context->parent;
    returnToParentContext(context);
}
