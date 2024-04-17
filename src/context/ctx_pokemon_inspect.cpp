#include "context/ctx_pokemon_inspect.hpp"
#include "context/context_enum.hpp"
#include "context/components/elements.hpp"
#include "core/constants/primary.hpp"
#include "context/components/animations.hpp"
#include "context/ctx_battle_view.hpp"

#define WINDOW_LEFT_PADDING 2
#define ENTRY_LIST_INITIAL_OFFSET 1
#define ENTRY_HEIGHT 4
#define ENTRY_HORIZONTAL_PADDING 3
#define MARGINS 1

PokemonInspectContext::PokemonInspectContext(
    AbstractContext* parent,
    const std::vector<std::shared_ptr<Pokemon>> &pokemonList
) : AbstractContext(
    ContextType::POKEMON_INSPECT_CONTEXT,
    parent,
    {
        MARGINS,
        MARGINS,
        parent->dimensions.width - 2 * MARGINS,
        parent->dimensions.height - BATTLE_CTX_FOOTER_SIZE - 2 * MARGINS
    }
) {
    this->pokemonList = pokemonList;
    this->numEntries = static_cast<int>(pokemonList.size());

    // Construct and switch to it
    constructWindow(false);

    // Add extra stuff
    windowTitle(this, "Press ESC/~ to return to battle", 2);
}


PokemonInspectContext::PokemonInspectContext(
    AbstractContext* parent,
    const std::vector<std::shared_ptr<Pokemon>> &friendlyPokemon,
    const std::vector<std::shared_ptr<Pokemon>> &opponentPokemon
) : PokemonInspectContext(parent, friendlyPokemon) {
    this->opponentPokemon = opponentPokemon;

    mvwaddstr(window, dimensions.height - 1, 2, "[ TAB to switch between friendly and opponent Pokemon ]");
}

void PokemonInspectContext::start() {
    AbstractContext::start();

    pokemonListEntry();

    returnToParentContext();
}

void PokemonInspectContext::pokemonListEntry() {
    int scroll = 0;

    while (true) {
        for (int i = 0; i < numEntries; i++) {
            int lineOffset = i * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET;

            std::shared_ptr<Pokemon> pokemon = pokemonList[i];

            spaces(this, 1, lineOffset, dimensions.width - 2);
            spaces(this, 1, lineOffset + ENTRY_HEIGHT, dimensions.width - 2);
            for (int k = 0; k <= ENTRY_HEIGHT; k++) {
                mvwaddch(window, lineOffset + k, 1, ' ');
                mvwaddch(window, lineOffset + k, dimensions.width - 2, ' ');
            }

            bool isSelected = scroll == i;

            // Pokemon name and level
            mvwprintw(
                window, lineOffset + 1, ENTRY_HORIZONTAL_PADDING, "[Lv. %d] %s",
                pokemon->level,
                ((isSelected ? "> " : "  ") + pokemon->name()).c_str()
            );

            // Pokemon stats
            mvwaddstr(window, lineOffset + 2, ENTRY_HORIZONTAL_PADDING, pokemon->statsString().c_str());

            // Pokemon moves
            mvwaddstr(window, lineOffset + 3, ENTRY_HORIZONTAL_PADDING, pokemon->movesString().c_str());

        }

        drawBox(
            window,
            1,
            scroll * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET,
            dimensions.width - 2,
            ENTRY_HEIGHT + 1
        );

        refreshContext();

        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
                return;  // Exit the loop

            case KEY_UP: {
                scroll = clamp(scroll - 1, 0, numEntries - 1);
                continue;
            }
            case KEY_DOWN: {
                scroll = clamp(scroll + 1, 0, numEntries - 1);
                continue;
            }
            default: {}
        }
    }
}

//void TrainerListContext::trainerListEntry() {
//    Player* player = GAME.player;
//
//    int numEntities = static_cast<int>(entityList->size());
//    int scroll = 0;
//    int maxScroll = max(numEntities - TRAINER_LIST_WINDOW_HEIGHT + 2, 0);
//    while (true) {
//        // List entities
//        for (int i = 1; i < min(numEntities, TRAINER_LIST_WINDOW_HEIGHT - 2); i++) {
//            CorporealEntity* ent = entityList->at(i + scroll);
//
//            // Initialize variables for string formatting
//            char entityString[TRAINER_LIST_WINDOW_WIDTH - 1];
//            char dxString[11];
//            char indexString[13];
//
//            // Format the dx part
//            sprintf(dxString, "dx=%d,", ent->mapX - player->mapX);
//            rightPad(dxString, 7);
//
//            // Format the index part
//            sprintf(indexString, "%d.", i + scroll);
//            rightPad(indexString, 5);
//
//            // Concatenate everything together
//            sprintf(
//                entityString, "%s%c @ (%s dy=%d)",
//                indexString,
//                entityToChar(ent),
//                dxString,
//                ent->mapY - player->mapY
//            );
//            rightPad(entityString, TRAINER_LIST_WINDOW_WIDTH - WINDOW_LEFT_PADDING - 1);
//
//            // Print it out
//            mvwprintw(window, i, WINDOW_LEFT_PADDING, "%s", entityString);
//        }
//
//        // Add an indicator if scroll is available
//        if (numEntities >= TRAINER_LIST_WINDOW_HEIGHT - 1) {
//            if (scroll != maxScroll) {
//                mvwprintw(
//                    window, TRAINER_LIST_WINDOW_HEIGHT - 2, WINDOW_LEFT_PADDING,
//                    "<%d MORE>  ", maxScroll - scroll
//                );
//            } else {
//                mvwprintw(
//                    window, TRAINER_LIST_WINDOW_HEIGHT - 2, WINDOW_LEFT_PADDING,
//                    "<END>      "
//                );
//            }
//
//        }

//    }
//}

