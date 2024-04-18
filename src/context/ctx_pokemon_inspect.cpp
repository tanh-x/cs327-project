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
#define LEFT_SIDE_WIDTH 60
#define RIGHT_SIDE_LEFT_OFFSET (LEFT_SIDE_WIDTH + 2)

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

    // Draw window skeleton
    renderWindowSkeleton();
}


PokemonInspectContext::PokemonInspectContext(
    AbstractContext* parent,
    const std::vector<std::shared_ptr<Pokemon>> &friendlyPokemon,
    const std::vector<std::shared_ptr<Pokemon>> &opponentPokemon
) : PokemonInspectContext(parent, friendlyPokemon) {
    this->opponentPokemon = opponentPokemon;
    this->hasSecondaryList = true;
}

void PokemonInspectContext::start() {
    AbstractContext::start();

    pokemonListEntry();

    returnToParentContext();
}

void PokemonInspectContext::pokemonListEntry() {
    int scrollIdx = 0;

    while (true) {
        // Draw the Pokemon entries
        for (int i = 0; i < numEntries; i++) {
            int lineOffset = i * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET;

            std::shared_ptr<Pokemon> pokemon = secondaryList ? opponentPokemon[i] : pokemonList[i];

            spaces(this, 1, lineOffset, LEFT_SIDE_WIDTH - 2);
            spaces(this, 1, lineOffset + ENTRY_HEIGHT, LEFT_SIDE_WIDTH - 2);
            for (int k = 0; k <= ENTRY_HEIGHT; k++) {
                mvwaddch(window, lineOffset + k, 1, ' ');
                mvwaddch(window, lineOffset + k, LEFT_SIDE_WIDTH - 2, ' ');
            }

            // Pokemon name and level
            mvwprintw(
                window, lineOffset + 1, ENTRY_HORIZONTAL_PADDING, "[Lv.%d | %c] %s (%s)",
                pokemon->level,
                pokemon->gender ? 'M' : 'F',
                pokemon->name.c_str(),
                pokemon->typesString().c_str()
            );

            // Pokemon stats
            mvwaddstr(window, lineOffset + 2, ENTRY_HORIZONTAL_PADDING, pokemon->statsString().c_str());

            pokemonHealthBar(this, pokemon, ENTRY_HORIZONTAL_PADDING, lineOffset + 3);

            // Print the health display
            mvwprintw(
                window, lineOffset + 3, ENTRY_HORIZONTAL_PADDING + 33,
                "HP: %d / %d",
                pokemon->health, pokemon->maxHp
            );
        }

        // Draw a box around the currently selected Pokemon
        drawBox(
            window,
            1,
            scrollIdx * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET,
            LEFT_SIDE_WIDTH - 2,
            ENTRY_HEIGHT + 1
        );

        // Get the pointer to the selected Pokemon
        std::shared_ptr<Pokemon> selectedPokemon = (secondaryList ? opponentPokemon[scrollIdx]
                                                                  : pokemonList[scrollIdx]);

        // If the current Pokemon is in battle, indicate it
        if (parent->type == ContextType::BATTLE_CONTEXT) {
            auto* battleCtx = dynamic_cast<BattleViewContext*>(parent);
            if (battleCtx->friendlyActive == selectedPokemon || battleCtx->opponentActive == selectedPokemon) {
                mvwaddstr(window, scrollIdx * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET, 3, " IN BATTLE ");
            } else if (!secondaryList && !selectedPokemon->isDead()) {
                // Also add a prompt for choosing the Pokemon
                mvwaddstr(
                    window,
                    scrollIdx * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET + ENTRY_HEIGHT,
                    LEFT_SIDE_WIDTH - 4 - 17,
                    " ENTER to choose "
                );
            }
        }


        // Draw its sprite in the lower right corner
        rasterizePokemonSprite(window, selectedPokemon->data->id, LEFT_SIDE_WIDTH + 11, 12, true);

        // Display further information in the upper right corner
        spaces(this, LEFT_SIDE_WIDTH + 2, 1, dimensions.width - LEFT_SIDE_WIDTH - 2);
        mvwaddstr(window, 1, LEFT_SIDE_WIDTH + 2, selectedPokemon->movesString().c_str());


        // We're done, so refresh
        refreshContext();

        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
                return;  // Exit the loop

            case '\n':
            case ' ': {
                // Check if we're in the right context
                if (parent->type != ContextType::BATTLE_CONTEXT) continue;

                // Check if the selected pokemon is the active pokemon
                auto* battleCtx = dynamic_cast<BattleViewContext*>(parent);
                if (battleCtx->friendlyActive == selectedPokemon) continue;

                // Check if we're on the secondary list
                if (secondaryList) continue;

                // Check if the Pokemon was knocked out
                if (selectedPokemon->isDead()) continue;

                // Then, we can take this Pokemon
                returnToParentContext();
                battleCtx->onPokemonSelect(scrollIdx);
                return;
            }

            case '\t':
                // Switch to the opponent pokemon list
                if (hasSecondaryList) {
                    secondaryList ^= true;  // Toggle it, if available
                    numEntries = static_cast<int>(secondaryList ? opponentPokemon.size() : pokemonList.size());

                    // Clear the entire window
                    wclear(window);

                    // Then redraw it
                    scrollIdx = 0;
                    renderWindowSkeleton();
                }

            case '8':
            case 'W':
            case 'w':
            case KEY_UP: {
                scrollIdx = clamp(scrollIdx - 1, 0, numEntries - 1);
                continue;
            }

            case '2':
            case 'S':
            case 's':
            case KEY_DOWN: {
                scrollIdx = clamp(scrollIdx + 1, 0, numEntries - 1);
                continue;
            }
            default: {}
        }
    }
}

void PokemonInspectContext::renderWindowSkeleton() {
    windowTitle(this, "Press ESC/~ to return to battle", 2);

    // Indicate which list is currently being displayed
    mvwprintw(
        window, dimensions.height - 1, 2, "[ Showing %s Pokemon. TAB to see %s ]",
        secondaryList ? "opponent's" : "your",
        !secondaryList ? "opponent's" : "yours"
    );

    mvwvline(window, 0, LEFT_SIDE_WIDTH, ACS_VLINE, dimensions.height);
    mvwhline(window, 11, LEFT_SIDE_WIDTH, ACS_HLINE, dimensions.width - LEFT_SIDE_WIDTH);
    mvwaddch(window, 11, LEFT_SIDE_WIDTH, ACS_LTEE);
    mvwaddch(window, 12, dimensions.width - 1, ' ');
}
