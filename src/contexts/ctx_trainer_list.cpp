#include <ncurses.h>
#include <cassert>
#include "contexts/ctx_trainer_list.hpp"
#include "graphics/artist.hpp"
#include "utils/string_helpers.hpp"
#include "core/input.hpp"
#include "contexts/components/elements.hpp"
#include "contexts/components/animations.hpp"

#define WINDOW_LEFT_PADDING 2

void startTrainerListWindow() {
    // Define dimensions of the new window
    Rect2D windowDimensions;
    windowDimensions.width = TRAINER_LIST_WINDOW_WIDTH;
    windowDimensions.height = TRAINER_LIST_WINDOW_HEIGHT;

    // Find the center of the parent window
    windowDimensions.x = (WINDOW_WIDTH - windowDimensions.width) / 2;
    windowDimensions.y = (WINDOW_HEIGHT - windowDimensions.height) / 2;

    // Do a fancy animation
    expandWindowVertical(windowDimensions, INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    AbstractContext* context = constructChildWindowContext(ContextType::TRAINER_LIST_CONTEXT, windowDimensions);

    // Add extra stuff
    windowTitle(context, "Trainer list");

    // Enter the main loop, which is where it will be wrefresh()'d
    trainerListEntry();

    // trainerListEntry() only returns upon exit, so we want to destroy the window and the context
    // First, swap the context back to the parent
    assert(GAME.context->parent != nullptr);
    GAME.context = GAME.context->parent;

    // Then, dispose it, and return back to the parent window.
    returnToParentContext(context);

    // We are done with the trainer list, so exit back to the call site
}

void trainerListEntry() {
    assert(GAME.context->type == ContextType::TRAINER_LIST_CONTEXT);
    WINDOW* window = GAME.context->window;
    std::vector<AbstractEntity*> entities = GAME.currentEntManager->entities;
    Player* player = GAME.player;

    int numEnities = int(entities.size());
    int scroll = 0;
    int maxScroll = numEnities - TRAINER_LIST_WINDOW_HEIGHT + 2;
    while (true) {
        // List entities
        for (int i = 1; i < min(numEnities, TRAINER_LIST_WINDOW_HEIGHT - 2); i++) {
            AbstractEntity* ent = entities[i + scroll];

            // Format the string
            char entityString[TRAINER_LIST_WINDOW_WIDTH - 1];
            char dxString[11];
            char indexString[13];
            sprintf(dxString, "dx=%d,", ent->mapX - player->mapX);
            rightPad(dxString, 7);
            sprintf(indexString, "%d.", i + scroll);
            rightPad(indexString, 5);
            sprintf(
                entityString, "%s%c @ (%s dy=%d)",
                indexString,
                entityToChar(ent),
                dxString,
                ent->mapY - player->mapY
            );
            rightPad(entityString, TRAINER_LIST_WINDOW_WIDTH - WINDOW_LEFT_PADDING - 1);

            // Print it out
            mvwprintw(window, i, WINDOW_LEFT_PADDING, "%s", entityString);
        }
        if (entities.size() >= TRAINER_LIST_WINDOW_HEIGHT) {
            if (scroll != maxScroll) {
                mvwprintw(
                    window, TRAINER_LIST_WINDOW_HEIGHT - 2, WINDOW_LEFT_PADDING,
                    "<%d MORE>  ", maxScroll - scroll
                );
            } else {
                mvwprintw(
                    window, TRAINER_LIST_WINDOW_HEIGHT - 2, WINDOW_LEFT_PADDING,
                    "<END>      "
                );
            }

        }
        wrefresh(window);

        // Listen for input
        int ch = getch();

        // Check it against our trainer list handlers
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
            case 't':  // Toggle alias
                return;  // Exit the loop

            case KEY_UP: {
                scroll = clamp(scroll - 1, 0, maxScroll);
                continue;
            }

            case KEY_DOWN: {
                scroll = clamp(scroll + 1, 0, maxScroll);
                continue;
            }

            default: {}
        }

        // If not, then propagate it through the global handler first
        if (handlePlayerInput()) continue;

        // If still not caught, then don't do anything
    }
}


bool trainerListInputHandler(int key) {
    // TODO: Integrate this into the system, if time allows
    return false;
}