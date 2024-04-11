#include <ncurses.h>
#include "context/ctx_trainer_list.hpp"
#include "graphics/artist.hpp"
#include "utils/string_helpers.hpp"
#include "core/input.hpp"
#include "context/components/elements.hpp"
#include "context/components/animations.hpp"

#define WINDOW_LEFT_PADDING 2

TrainerListContext::TrainerListContext(
    MainContext* parent,
    std::vector<AbstractEntity*>* entityList
) : AbstractContext(
    ContextType::TRAINER_LIST_CONTEXT,
    parent,
    {
        (WINDOW_WIDTH - TRAINER_LIST_WINDOW_WIDTH) / 2,
        (WINDOW_HEIGHT - TRAINER_LIST_WINDOW_HEIGHT) / 2,
        TRAINER_LIST_WINDOW_WIDTH,
        TRAINER_LIST_WINDOW_HEIGHT,
    }
) {
    // Save the pointer to the entity list
    this->entityList = entityList;

    // Do a funny animation
    verticalExpandAnimation(dimensions, INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow();

    // Add extra stuff
    windowTitle(this, "Trainer list", 2);
}

void TrainerListContext::start() {
    AbstractContext::start();

    // Enter the main loop, which is where it will be wrefresh()'d
    trainerListEntry();
    // trainerListEntry() only returns upon exit, so we want to destroy the window and the context

    // Swap the context back to the parent
    returnToParentContext();
}

void TrainerListContext::trainerListEntry() {
    Player* player = GAME.player;

    int numEntities = static_cast<int>(entityList->size());
    int scroll = 0;
    int maxScroll = max(numEntities - TRAINER_LIST_WINDOW_HEIGHT + 2, 0);
    while (true) {
        // List entities
        for (int i = 1; i < min(numEntities, TRAINER_LIST_WINDOW_HEIGHT - 2); i++) {
            AbstractEntity* ent = entityList->at(i + scroll);

            // Initialize variables for string formatting
            char entityString[TRAINER_LIST_WINDOW_WIDTH - 1];
            char dxString[11];
            char indexString[13];

            // Format the dx part
            sprintf(dxString, "dx=%d,", ent->mapX - player->mapX);
            rightPad(dxString, 7);

            // Format the index part
            sprintf(indexString, "%d.", i + scroll);
            rightPad(indexString, 5);

            // Concatenate everything together
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

        // Add an indicator if scroll is available
        if (numEntities >= TRAINER_LIST_WINDOW_HEIGHT - 1) {
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

        refreshContext();

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