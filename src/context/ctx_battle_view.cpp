#include <ncurses.h>
#include <unistd.h>
#include "context/ctx_battle_view.hpp"
#include "core/game_manager.hpp"
#include "graphics/artist.hpp"
#include "context/components/animations.hpp"
#include "context/components/elements.hpp"
#include "utils/string_helpers.hpp"
#include "context/ctx_pokemon_inspect.hpp"
#include "graphics/ncurses_artist.hpp"


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

    leftPokemon = GAME.pokemonInventory.at(0);
    rightPokemon = opponent->pokemonInventory.at(0);

    // Play a funny animation
    battleTransitionAnimation(INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow(true);

    // Time to build the dialog window

    // Define the dimensions of the dialog window
    Rect2D dialogRect;
    dialogRect.x = 1;
    dialogRect.y = WINDOW_HEIGHT - BATTLE_CTX_DIALOG_WINDOW_HEIGHT - 3;
    dialogRect.width = WINDOW_WIDTH - 2;
    dialogRect.height = BATTLE_CTX_DIALOG_WINDOW_HEIGHT + 3;

    // Play an animation of the pokemon entering
    pokemonEntryAnimation();

    // Start with an animation
    verticalExpandAnimation(dialogRect, INTERVAL_30FPS_MICROS);

    // Then build the actual dialog window
    this->dialogWindow = newwin(dialogRect.height, dialogRect.width, dialogRect.y, dialogRect.x);
    keypad(dialogWindow, true);
    box(dialogWindow, 0, 0);

    // Print out text to the dialog window
    if (opponent->type == EntityEnum::POKEMON_VESSEL) {
        mvwprintw(dialogWindow, 1, 1, "A wild %s appears!", opponent->pokemonInventory[0]->name.c_str());
    } else {
        mvwprintw(dialogWindow, 1, 1, "%s wants to fight!", opponent->name);
    }

    // Print the pokemon list
    std::string pokemonList;
    for (auto &pokemon: opponent->pokemonInventory) {
        pokemonList += pokemon->name + " ";
    }
    mvwprintw(dialogWindow, 2, 1, "Pokemon list: %s", pokemonList.c_str());

    mvwaddstr(dialogWindow, dialogRect.height - 3, WINDOW_WIDTH - 12, "ENTER ");
    wattron(dialogWindow, A_BLINK);
    waddstr(dialogWindow, ">>");
    wattroff(dialogWindow, A_BLINK);

    // We're done with building the window
    wrefresh(dialogWindow);
    refreshContext();

    // Only accepts ESC/ENTER/SPACE to close the dialog
    while (true) {
        int ch = getch();
        if (ch == ESCAPE_KEY || ch == '\n' || ch == ' ') break;
    }

    delwin(dialogWindow);
    touchwin(window);
    refreshContext();

}

void BattleViewContext::start() {
    AbstractContext::start();

    battleContextLoop();

    // If we got here, ESC has been pressed
    opponent->activeBattle = false;
    returnToParentContext();
}

void BattleViewContext::battleContextLoop() {
    int sectionOffset = dimensions.width - PROMPT_ACTION_WIDTH + 1;

    int prompt = 0;
    std::string mainActions[] = {"FIGHT", "BAG", "POKEMON", "RUN (butchers the NPC)"};
    std::string* currentPromptList = mainActions;

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        renderWindowSkeleton();
        renderPokemon();

        // Draw the Pokemon and their status bar

        // Draw the action prompts on the lower right corner
        for (int line = 0; line < PROMPT_ACTIONS; line++) {
            std::string lineStr =
                (prompt == line ? " > " : "   ")
                + (std::string) currentPromptList[line]
                + (prompt == line ? " <" : " ");

            mvwaddstr(
                window, FOOTER_OFFSET + 1 + line, sectionOffset + 1,
                rightPad(lineStr, PROMPT_ACTION_WIDTH - 3).c_str()
            );
        }

        refreshContext();

        // Wait for user input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
                return;  // Exit the loop

            case KEY_UP:
            case 'w': prompt = clamp(prompt - 1, 0, PROMPT_ACTIONS - 1);
                continue;

            case KEY_DOWN:
            case 's': prompt = clamp(prompt + 1, 0, PROMPT_ACTIONS - 1);
                continue;

            case '\n':
            case ' ': {
                // Prompt action triggered
                if (prompt == 0) {
                    // FIGHT

                } else if (prompt == 1) {
                    // BAG

                } else if (prompt == 2) {
                    // POKEMON

                    auto* inspectContext = new PokemonInspectContext(
                        this,
                        GAME.pokemonInventory,
                        opponent->pokemonInventory
                    );
                    inspectContext->start();
                    refresh();
                    continue;
                } else if (prompt == 3) {
                    // RUN
                    return;
                } else continue;
            }
        }
    }
}

void BattleViewContext::renderWindowSkeleton() {
    box(window, 0, 0);
    horizontalSeparator(this, 0, FOOTER_OFFSET, WINDOW_WIDTH);
    verticalSeparator(this, WINDOW_WIDTH - PROMPT_ACTION_WIDTH + 1, FOOTER_OFFSET, BATTLE_CTX_FOOTER_SIZE + 1);
    refreshContext();
}

void BattleViewContext::renderPokemon() {
    rasterizePokemonSprite(window, leftPokemon->data->id, POKEMON_LEFT_X, POKEMON_LEFT_Y, true);
    rasterizePokemonSprite(window, rightPokemon->data->id, POKEMON_RIGHT_X, POKEMON_RIGHT_Y, false);
}


void BattleViewContext::pokemonEntryAnimation() {
    for (int i = -36; i <= 0; i += 3) {
        wclear(window);
        rasterizePokemonSprite(window, leftPokemon->data->id, POKEMON_LEFT_X + i, POKEMON_LEFT_Y, true);
        rasterizePokemonSprite(window, rightPokemon->data->id, POKEMON_RIGHT_X - i, POKEMON_RIGHT_Y, false);
        usleep(INTERVAL_30FPS_MICROS);
        box(window, 0, 0);
        refreshContext();
    }
}


