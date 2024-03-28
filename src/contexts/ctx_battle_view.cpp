#include <ncurses.h>
#include <unistd.h>
#include "contexts/ctx_battle_view.hpp"
#include "core/game_manager.hpp"
#include "graphics/artist.hpp"
#include "contexts/components/animations.hpp"

BattleViewContext::BattleViewContext(
    AbstractContext* parent,
    AbstractEntity* opponent
) : AbstractContext(
    ContextType::BATTLE_CONTEXT,
    parent,
    {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}
) {
    // Store the pointer to the opponent first
    this->opponent = opponent;

    // Play a funny animation
    battleTransition(INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow();

    // Add a placeholder title
    mvwprintw(window, 1, 2, "PLACEHOLDER BATTLE INTERFACE");
    // We're done with initializing the main window

    // Refresh the window once before adding the dialog window
    refreshContext();

    // Wait a little bit to create a delay before the dialog animation starts
    usleep(STD_SLOW_FRAME_DELAY);

    // Time to build the dialog window

    // Define the dimensions of the dialog window
    Rect2D dialogRect;
    dialogRect.x = 1;
    dialogRect.y = WINDOW_HEIGHT - DIALOG_WINDOW_HEIGHT;
    dialogRect.width = WINDOW_WIDTH - 2;
    dialogRect.height = DIALOG_WINDOW_HEIGHT;

    // Start with an animation
    expandWindowVertical(dialogRect, INTERVAL_30FPS_MICROS);

    // Then build the actual dialog window
    WINDOW* dialogWindow = newwin(dialogRect.height, dialogRect.width, dialogRect.y, dialogRect.x);
    keypad(dialogWindow, true);
    box(dialogWindow, 0, 0);

    // Print out text to the dialog window
    mvwprintw(
        dialogWindow, 1, 1, "%s wants to fight!",
        opponent->name
    );
    mvwprintw(
        dialogWindow, DIALOG_WINDOW_HEIGHT - 2, 1, "<Opponent: \"%s\" (type %c) [@ %p]>",
        opponent->name, entityToChar(opponent), &opponent
    );

    // We're done with building the window
    refreshContext();
}

void BattleViewContext::start() {
    AbstractContext::start();

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == ESCAPE_KEY || ch == '`' || ch == '~') break;
    }

    // If we got here, ESC has been pressed
    opponent->activeBattle = false;
    returnToParentContext();
}


