#include <ncurses.h>
#include "contexts/ctx_trainer_list.h"
#include "graphics/artist.h"
#include "utils/string_helpers.h"

void startTrainerListWindow(GameManager* game) {
    WINDOW* parentWindow = stdscr;
    int width = TRAINER_LIST_WINDOW_WIDTH;
    int height = TRAINER_LIST_WINDOW_HEIGHT;

    // Find the center of the window
    int top = (WINDOW_HEIGHT - height) / 2;
    int left = (WINDOW_WIDTH - width) / 2;

    // Instantiate new window
    WINDOW* window = newwin(height, width, top, left);

    // Draw a box around the window
    box(window, 0, 0);
    keypad(window, TRUE);

    // We're done with initialization
    game->context = TRAINER_LIST_CONTEXT;
    wrefresh(window);

    // Enter the main loop
    trainerListEntry(game, window, parentWindow);

    // trainerListEntry() only returns upon exit, so we destroy the window and exit
    disposeTrainerListWindow(window, parentWindow);
    game->context = WORLD_CONTEXT;
}

void trainerListEntry(GameManager* game, WINDOW* window, WINDOW* parentWindow) {
    ArrayList* entities = game->entManager->entities;
    Player* player = game->player;

    int scroll = 0;
    int maxScroll = entities->size - TRAINER_LIST_WINDOW_HEIGHT + 2;
    while (true) {
        // List entities
        for (int i = 1; i < min(entities->size, TRAINER_LIST_WINDOW_HEIGHT - 2); i++) {
            Entity* ent = entities->array[i + scroll];

            // Format the string
            char entityString[TRAINER_LIST_WINDOW_WIDTH - 1];
            char dxString[11];
            char indexString[13];
            sprintf(dxString, "dx=%d,", ent->mapX - player->mapX);
            rightPad(dxString, 7);
            sprintf(indexString, "%d.", i + scroll);
            rightPad(indexString, 4);
            sprintf(
                entityString, "%s%c @ (%s dy=%d)",
                indexString,
                entityToChar(ent),
                dxString,
                ent->mapY - player->mapY
            );
            rightPad(entityString, TRAINER_LIST_WINDOW_WIDTH - 2);

            // Print it out
            mvwprintw(window, i, 1, "%s", entityString);
        }
        if (entities->size >= TRAINER_LIST_WINDOW_HEIGHT) {
            if (scroll != maxScroll) {
                mvwprintw(window, TRAINER_LIST_WINDOW_HEIGHT - 2, 1, "<%d MORE>  ", maxScroll - scroll);
            } else {
                mvwprintw(window, TRAINER_LIST_WINDOW_HEIGHT - 2, 1, "<END>      ");
            }

        }
        wrefresh(window);


        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case 't':  // Toggle alias
                return;

            case KEY_UP: {
                scroll = clamp(scroll - 1, 0, maxScroll);
                break;
            }

            case KEY_DOWN: {
                scroll = clamp(scroll + 1, 0, maxScroll);
                break;
            }

            default: {}
        }
    }
}

void disposeTrainerListWindow(WINDOW* window, WINDOW* parentWindow) {
    delwin(window);
    wrefresh(parentWindow);
}

bool trainerListInputHandler(GameManager* game, GameOptions* options, int key) {
    // TODO: Integrate this into the system, if time allows
    return false;
}