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
#include "context/ctx_inventory.hpp"


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

    int firstPokemon = -1;
    for (int i = 0; i < static_cast<int>(GAME.pokemonInventory.size()); i++) {
        if (GAME.pokemonInventory.at(i)->isDead()) continue;
        firstPokemon = i;
        break;
    }

    if (firstPokemon == -1) {
        // Heal the first Pokemon by 1 HP
        friendlyActive = GAME.pokemonInventory.at(0);
        friendlyActive->heal(1);
    } else {
        friendlyActive = GAME.pokemonInventory.at(firstPokemon);
    }

    opponentActive = opponent->pokemonInventory.at(0);

    // Instantiate the manager
    manager = new BattleManager(this);
    // TODO: Memory?

    // Play a funny animation
    battleTransitionAnimation(INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow(true);

    // Play an animation of the pokemon entering
    pokemonEntryAnimation();


    // Define the dimensions of the two health bar
    leftHealthBarRect = {
        POKEMON_LEFT_X + 1,
        POKEMON_LEFT_Y - HEALTH_BAR_HEIGHT - HEALTH_BAR_VERTICAL_MARGIN + 1,
        HEALTH_BAR_WIDTH,
        HEALTH_BAR_HEIGHT
    };
    rightHealthBarRect = {
        POKEMON_RIGHT_X - 3,
        POKEMON_RIGHT_Y + SPRITE_HEIGHT + HEALTH_BAR_VERTICAL_MARGIN - 1,
        HEALTH_BAR_WIDTH,
        HEALTH_BAR_HEIGHT
    };

    // Health bar animation and render them
    verticalExpandAnimation(leftHealthBarRect, INTERVAL_30FPS_MICROS);
    renderHealthBar(true);

    verticalExpandAnimation(rightHealthBarRect, INTERVAL_30FPS_MICROS);
    renderHealthBar(false);

    // Define the dimensions of the dialog window
    this->dialogRect.x = 2;
    this->dialogRect.y = FOOTER_OFFSET + 1;
    this->dialogRect.width = dimensions.width - PROMPT_ACTION_WIDTH - 2;
    this->dialogRect.height = BATTLE_CTX_FOOTER_SIZE - 1;
    this->dialogWindow = newwin(dialogRect.height, dialogRect.width, dialogRect.y, dialogRect.x);
    // Build the dialog window
    keypad(this->dialogWindow, true);

    // Define the dimensions of the dialog window
    Rect2D popupRect;
    popupRect.x = 1;
    popupRect.y = WINDOW_HEIGHT - BATTLE_CTX_DIALOG_WINDOW_HEIGHT;
    popupRect.width = WINDOW_WIDTH - 2;
    popupRect.height = BATTLE_CTX_DIALOG_WINDOW_HEIGHT;

    // Then build the popup window

    // Start with an animation
    verticalExpandAnimation(popupRect, INTERVAL_30FPS_MICROS);

    // Build the popup window
    WINDOW* popup = newwin(popupRect.height, popupRect.width, popupRect.y, popupRect.x);
    keypad(popup, true);
    box(popup, 0, 0);

    // Print out text to the popup window
    if (opponent->type == EntityEnum::POKEMON_VESSEL) {
        mvwprintw(popup, 1, 1, "A wild %s appears!", opponent->pokemonInventory[0]->name.c_str());
    } else {
        mvwprintw(popup, 1, 1, "%s wants to fight!", opponent->name);
    }

    // Print the pokemon list
    std::string pokemonList;
    for (auto &pokemon: opponent->pokemonInventory) {
        pokemonList += pokemon->name + ", ";
    }
    mvwprintw(popup, 2, 1, "Pokemon list: %s", pokemonList.substr(0, pokemonList.size() - 2).c_str());

    mvwaddstr(popup, popupRect.height - 3, WINDOW_WIDTH - 12, "ENTER ");
    wattron(popup, A_BLINK);
    waddstr(popup, ">>");
    wattroff(popup, A_BLINK);

    // We're done with building the window
    wrefresh(popup);
    refreshContext();

    // Only accepts ESC/ENTER/SPACE to close the popup
    while (true) {
        int ch = getch();
        if (ch == ESCAPE_KEY || ch == '\n' || ch == ' ') break;
    }

    delwin(popup);
    touchwin(window);
    refreshContext();
}

void BattleViewContext::start() {
    AbstractContext::start();

    battleContextLoop();

    opponent->activeBattle = false;
    returnToParentContext();
}

void BattleViewContext::battleContextLoop() {
    int sectionOffset = dimensions.width - PROMPT_ACTION_WIDTH + 1;

    int prompt = 0;
    std::string mainActions[] = {"FIGHT", "BAG", "POKEMON", "RUN"};
    auto* currentPromptList = static_cast<std::string*>(mainActions);

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        renderWindowSkeleton();

        // Draw the Pokemon and their status bar
        renderPokemon();
        renderHealthBar(true);
        renderHealthBar(false);

        // Draw the action prompts in the lower right corner
        for (int line = 0; line < PROMPT_ACTIONS; line++) {
            // Add > < if selected
            std::string lineStr =
                (prompt == line ? " > " : "   ")
                + (std::string) currentPromptList[line]
                + (prompt == line ? " <" : " ");

            mvwaddstr(
                window, FOOTER_OFFSET + 1 + line, sectionOffset + 1,
                rightPad(lineStr, PROMPT_ACTION_WIDTH - 3).c_str()
            );
        }

        // Reminder to choose another Pokemon
        if (friendlyActive->isDead()) writeToDialog({"Choose another Pokemon to continue the battle."}, false);

        refreshContext();

        if (battleTerminated) {
            if (pokemonCaptured) {
                GAME.pokemonInventory.push_back(opponentActive);
                writeToDialog({"You captured a " + opponentActive->name + "!"}, true);
            } else {
                writeToDialog(
                    {playerVictory
                     ? "You defeated " + std::string(opponent->name) + "!"
                     : "You lost the Pokemon battle against " + std::string(opponent->name) + "!"
                    }, true
                );
            }
            usleep(200000);
            return;
        }

        // Wait for user input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w': prompt = clamp(prompt - 1, 0, PROMPT_ACTIONS - 1);
                continue;

            case KEY_DOWN:
            case 's': prompt = clamp(prompt + 1, 0, PROMPT_ACTIONS - 1);
                continue;

            case 'z':
            case 'Z':battleTerminated = true;
                playerVictory = true;
                continue;

            case 'x':
            case 'X':battleTerminated = true;
                playerVictory = false;
                continue;

            case '\n':
            case ' ': {
                // Prompt action triggered
                if (prompt == 0) {
                    // FIGHT
                    renderMoveList();
                    refreshContext();
                    continue;

                } else if (prompt == 1) {
                    // BAG
                    auto* inventoryContext = new InventoryContext(this);
                    inventoryContext->start();
                    refresh();
                    continue;
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
                    if (opponent->type == EntityEnum::POKEMON_VESSEL) return;

                    writeToDialog(
                        {
                            "You can only flee from a wild Pokemon",
                            "(Press Z for force win, X for force loss)"
                        }, true);
                    continue;
                } else continue;
            }

            default: continue;
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
    // Render Pokemon sprites
    rasterizePokemonSprite(window, friendlyActive->data->id, POKEMON_LEFT_X, POKEMON_LEFT_Y, true);
    rasterizePokemonSprite(window, opponentActive->data->id, POKEMON_RIGHT_X, POKEMON_RIGHT_Y, false);
    renderWindowSkeleton();
}


void BattleViewContext::renderHealthBar(bool friendly) {
    Rect2D rect = friendly ? leftHealthBarRect : rightHealthBarRect;
    std::shared_ptr<Pokemon> pokemon = friendly ? friendlyActive : opponentActive;

    // Draw the surrounding box around the health bar
    drawBox(window, rect);

    // Clear the content of the health bar
    for (int i = 0; i < rect.height - 2; i++) {
        spaces(window, rect.x + 1, rect.y + 1, rect.width - 2);
    }

    // Display the name, level, and type
    mvwprintw(
        window, rect.y + 1, rect.x + 2, "[Lv.%d] %s   ",
        pokemon->level,
        pokemon->name.c_str()
    );

    // Display the health value
    mvwprintw(
        window, rect.y + 2, rect.x + 2, "HP: %d / %d   ",
        pokemon->health, pokemon->maxHp
    );

    // Draw the health bar
    pokemonHealthBar(this, pokemon, rect.x + 2, rect.y + 3);
}


void BattleViewContext::pokemonEntryAnimation() {
    for (int i = -36; i <= 0; i += 3) {
        rasterizePokemonSprite(window, friendlyActive->data->id, POKEMON_LEFT_X + i, POKEMON_LEFT_Y, true);
        rasterizePokemonSprite(window, opponentActive->data->id, POKEMON_RIGHT_X - i, POKEMON_RIGHT_Y, false);
        usleep(INTERVAL_30FPS_MICROS);
        box(window, 0, 0);
        refreshContext();
    }
}


void BattleViewContext::pokemonSwapAnimation(int oldId, int newId, bool left) {
    int x = left ? POKEMON_LEFT_X : POKEMON_RIGHT_X;
    int y = left ? POKEMON_LEFT_Y : POKEMON_RIGHT_Y;

    for (int i = 0; i >= -36; i -= 3) {
        rasterizePokemonSprite(window, oldId, x + (left ? 1 : -1) * i, y, left);
        usleep(INTERVAL_30FPS_MICROS);
        box(window, 0, 0);
        refreshContext();
    }

    for (int i = -36; i <= 0; i += 3) {
        rasterizePokemonSprite(window, newId, x + (left ? 1 : -1) * i, y, left);
        usleep(INTERVAL_30FPS_MICROS);
        box(window, 0, 0);
        refreshContext();
    }

    box(window, 0, 0);
}


void BattleViewContext::pokemonShakeAnimation(bool left) {
    int x = left ? POKEMON_LEFT_X : POKEMON_RIGHT_X;
    int y = left ? POKEMON_LEFT_Y : POKEMON_RIGHT_Y;
    int id = left ? friendlyActive->data->id : opponentActive->data->id;

    for (int i = 0; i < 8; i++) {
        rasterizePokemonSprite(window, id, x + (i % 3 - 1) * 3, y, left);
        usleep(INTERVAL_30FPS_MICROS * 2);
        box(window, 0, 0);
        refreshContext();
    }

    box(window, 0, 0);
}


void BattleViewContext::renderMoveList() {
    Rect2D rect = {
        dimensions.width - PROMPT_ACTION_WIDTH + 1,
        FOOTER_OFFSET,
        PROMPT_ACTION_WIDTH - 1,
        PROMPT_ACTIONS + 2
    };

    // Start with an animation
    verticalExpandAnimation(rect, INTERVAL_30FPS_MICROS);

    // Build the window
    WINDOW* newWindow = newwin(rect.height, rect.width, rect.y, rect.x);
    keypad(newWindow, true);
    box(newWindow, 0, 0);
    wrefresh(newWindow);

    int moveScroll = 0;
    int numMoves = static_cast<int>(friendlyActive->moveSet.size());

    while (true) {
        // Add move prompts
        mvwaddstr(newWindow, 1, 1, moveScroll == 0 ? " > [CANCEL] <" : "   [CANCEL]  ");
        for (int line = 1; line <= numMoves; line++) {
            std::shared_ptr<MovesData> move = friendlyActive->moveSet[line - 1];

            // Add > < if selected
            std::string lineStr =
                (moveScroll == line ? " > " : "   ")
                + unkebabString(move->identifier)
                + (moveScroll == line ? " <" : " ");

            mvwaddstr(newWindow, line + 1, 1, rightPad(lineStr, PROMPT_ACTION_WIDTH - 3).c_str());
        }

        std::shared_ptr<MovesData> selectedMove = nullptr;
        if (moveScroll > 0 && moveScroll <= numMoves) {
            selectedMove = friendlyActive->moveSet[moveScroll - 1];
        }

        wrefresh(newWindow);
        if (selectedMove != nullptr) writeToDialog({selectedMove->toString()}, false);

        int ch = getch();
        switch (ch) {
            case '\n':
            case ' ': {
                bool success = false;
                // Submit the move if the prompt is selecting one
                if (moveScroll != 0 && selectedMove != nullptr) {
                    success = manager->submitPokemonMove(friendlyActive, selectedMove);
                }
                // Regardless, close the move window
                delwin(newWindow);
                renderWindowSkeleton();
                refreshContext();

                // If the move submission was successful, immediately start the turn computation
                if (success) manager->executeTurn();
                return;
            }

            case '8':
            case 'W':
            case 'w':
            case KEY_UP: {
                moveScroll = clamp(moveScroll - 1, 0, numMoves);
                continue;
            }

            case '2':
            case 'S':
            case 's':
            case KEY_DOWN: {
                moveScroll = clamp(moveScroll + 1, 0, numMoves);
                continue;
            }
            default: {}
        }
    }
}

void BattleViewContext::onPokemonSelect(int idx) {
    // Return to this context
    renderWindowSkeleton();
    renderPokemon();
    refreshContext();

    bool success = manager->submitPokemonChange(idx);

    if (!success) {
        writeToDialog({"Cannot change to this Pokemon"}, true);
        return;
    }

    // If our current Pokemon is dead, immediately swap the Pokemon out without costing a turn
    if (friendlyActive->isDead()) manager->swapPlayerPokemon(idx);
    else manager->executeTurn();  // Otherwise, this is a turn
}

void BattleViewContext::writeToDialog(const std::vector<std::string> &lines, bool animate) {
    for (int i = 0; i < dialogRect.height; i++) {
        spaces(dialogWindow, 1, i, dialogRect.width);
    }

    if (!animate) {
        for (auto &line: lines) mvwaddstr(dialogWindow, 0, 0, line.c_str());
        wrefresh(dialogWindow);
        return;
    }

    wmove(dialogWindow, 0, 0);
    for (auto &line: lines) {
        for (size_t i = 0; i < line.length(); i += DIALOG_CHAR_PER_FRAME) {
            std::string part = line.substr(i, DIALOG_CHAR_PER_FRAME);
            waddstr(dialogWindow, part.c_str());
            wrefresh(dialogWindow);
            usleep(DIALOG_FRAME_TIME);
        }
        waddch(dialogWindow, '\n');
    }

    usleep(DIALOG_WAIT_TIME * lines.size());
}

void BattleViewContext::tryCapturePokemon() {
    // Check if it's a wild pokemon
    if (opponent->type != EntityEnum::POKEMON_VESSEL) {
        writeToDialog({"You cannot steal somebody else's Pokemon!"}, true);
        return;
    }

    GAME.player->numPokeballs--;
    manager->submitItemTurn();

    if (proba() < POKEMON_CAPTURE_PROBABILITY) {
        battleTerminated = true;
        playerVictory = true;
        pokemonCaptured = true;
        return;
    }


    // Else, it was a skipped turn
    writeToDialog({"You failed to capture the " + opponentActive->name + "."}, true);
    manager->executeTurn();
}
