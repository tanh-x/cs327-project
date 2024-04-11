
#include "context/ctx_pokemon_inspect.hpp"
#include "context/context_enum.hpp"
#include "context/components/elements.hpp"
#include "core/constants/primary.hpp"
#include "context/components/animations.hpp"

#define WINDOW_LEFT_PADDING 2
#define LEFT_SIDEBAR_WIDTH 30

PokemonInspectContext::PokemonInspectContext(
    AbstractContext* parent,
    std::vector<std::shared_ptr<Pokemon>> friendlyPokemon,
    std::vector<std::shared_ptr<Pokemon>> opponentPokemon
) : AbstractContext(
    ContextType::POKEMON_INSPECT_CONTEXT,
    parent,
    {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - 5}
) {
    this->friendlyPokemon = friendlyPokemon;
    this->opponentPokemon = opponentPokemon;
    this->hasOpponent = !opponentPokemon.empty();
    this->opponentOffset = friendlyPokemon.size();
    this->maxScroll = opponentOffset + opponentPokemon.size();

    // Do a funny animation
    verticalExpandAnimation(dimensions, INTERVAL_60FPS_MICROS);

    // Construct and switch to it
    constructWindow();

    // Add extra stuff
    windowTitle(this, "Pokemon List", 2);

    verticalSeparator(this, LEFT_SIDEBAR_WIDTH, 0, dimensions.height);
}

void PokemonInspectContext::start() {
    AbstractContext::start();

    pokemonListEntry();

    returnToParentContext();
}

void PokemonInspectContext::pokemonListEntry() {
    int scroll = 0;

    while (true) {

        refreshContext();

        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
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

