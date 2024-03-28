#include <ncurses.h>
#include "context/ctx_building.hpp"
#include "context/components/animations.hpp"

BuildingContext::BuildingContext(MainContext* parent, TileType type) : AbstractContext(
    ContextType::BUILDING_CONTEXT,
    parent,
    {3, 3, WINDOW_WIDTH - 6, WINDOW_HEIGHT - 6}
) {
    // Do a fancy animation
    expandWindowVertical(dimensions, INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow();

    // Write some text based on the type of building it was
    if (type == POKEMART) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMART INTERFACE");
    else if (type == POKECENTER) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMON CENTER INTERFACE");

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
