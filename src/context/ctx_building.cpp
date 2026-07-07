#include <ncurses.h>
#include "context/ctx_building.hpp"
#include "context/components/animations.hpp"

BuildingContext::BuildingContext(MainContext* parent, TileType type) : AbstractContext(
    ContextType::BUILDING_CONTEXT,
    parent,
    {3, 3, WINDOW_WIDTH - 6, WINDOW_HEIGHT - 6}
) {
    // Do a fancy animation
    verticalExpandAnimation(dimensions, INTERVAL_60FPS_MICROS);

    // Construct and switch to it
    constructWindow(true);

    // Write some text based on the type of building it was
    if (type == POKEMART) {
        mvwprintw(window, 1, 1, "POKEMART - Resupplied all items");
        GAME.player->numPotions = ITEM_STARTING_AMOUNT;
        GAME.player->numRevives = ITEM_STARTING_AMOUNT;
        GAME.player->numPokeballs = ITEM_STARTING_AMOUNT;
    } else if (type == POKECENTER) {
        mvwprintw(window, 1, 1, "POKECENTER - All Pokemon healed");
        for (auto &pokemon: GAME.pokemonInventory) {
            pokemon->health = pokemon->maxHp;
        }
    }

    // We're done with initialization
}

void BuildingContext::start() {
    AbstractContext::start();

    // Only accepts `<`/`[` to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == '<' || ch == '[') break;
    }

    // If we got here, </[ has been pressed

    returnToParentContext();
}
