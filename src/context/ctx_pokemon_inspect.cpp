#include "context/ctx_pokemon_inspect.hpp"
#include "context/context_enum.hpp"
#include "context/components/elements.hpp"
#include "core/constants/primary.hpp"
#include "context/components/animations.hpp"
#include "context/ctx_battle_view.hpp"
#include "graphics/ncurses_artist.hpp"

#define WINDOW_LEFT_PADDING 2
#define ENTRY_LIST_INITIAL_OFFSET 1
#define ENTRY_HEIGHT 4
#define ENTRY_HORIZONTAL_PADDING 3
#define MARGINS 1
#define LEFT_SIDE_WIDTH 50
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
    redrawWindow();
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
    int scroll = 0;

    while (true) {
        // Draw the Pokemon entries
        for (int i = 0; i < static_cast<int>(secondaryList ? opponentPokemon.size() : pokemonList.size()); i++) {
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

            // Pokemon HP
            mvwaddch(window, lineOffset + 3, ENTRY_HORIZONTAL_PADDING, '[');
            mvwaddch(window, lineOffset + 3, ENTRY_HORIZONTAL_PADDING + 31, ']');
            float healthBarSteps = static_cast<float>(30 * pokemon->health) / static_cast<float>(pokemon->maxHp);
            sequentialColoredBar(
                this, ENTRY_HORIZONTAL_PADDING + 1, lineOffset + 3,
                30, healthBarSteps,
                RDYLGN10_PALETTE_OFFSET, RDYLGN10_PALETTE_COUNT,
                true, '=', ' '
            );
            mvwprintw(
                window, lineOffset + 3, ENTRY_HORIZONTAL_PADDING + 33,
                "HP: %d / %d",
                pokemon->health, pokemon->maxHp
            );

        }

        // Draw a box around the currently selected Pokemon
        if (!secondaryList) {
            drawBox(
                window,
                1,
                scroll * ENTRY_HEIGHT + ENTRY_LIST_INITIAL_OFFSET,
                LEFT_SIDE_WIDTH - 2,
                ENTRY_HEIGHT + 1
            );
        }

        // We're done, so refresh
        refreshContext();

        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
                return;  // Exit the loop

            case '\t':
                // Switch to the opponent pokemon list
                if (hasSecondaryList) {
                    secondaryList ^= true;  // Toggle it, if available

                    // Clear the entire window
                    wclear(window);

                    // Then redraw it
                    redrawWindow();
                }

            case '8':
            case 'W':
            case 'w':
            case KEY_UP: {
                scroll = clamp(scroll - 1, 0, numEntries - 1);
                continue;
            }

            case '2':
            case 'S':
            case 's':
            case KEY_DOWN: {
                scroll = clamp(scroll + 1, 0, numEntries - 1);
                continue;
            }
            default: {}
        }
    }
}

void PokemonInspectContext::redrawWindow() {
    windowTitle(this, "Press ESC/~ to return to battle", 2);

    // Indicate which list is currently being displayed
    mvwprintw(
        window, dimensions.height - 1, 2, "[ Showing %s Pokemon. TAB to switch ]",
        secondaryList ? "opponent's" : "your"
    );

    verticalSeparator(this, LEFT_SIDE_WIDTH, 0, dimensions.height);
}