#include <ncurses.h>
#include "contexts/ctx_building.hpp"
#include "contexts/components/animations.hpp"

BuildingContext::BuildingContext(WorldContext* parent, TileType type) : AbstractContext(
    ContextType::BUILDING_CONTEXT,
    parent,
    getDimensions()
) {
    // Do a fancy animation
    expandWindowVertical(dimensions, INTERVAL_30FPS_MICROS);

    // Construct and switch to it
    constructWindow();

    // Write some text based on the type of building it was
    if (type == POKEMART) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMART INTERFACE");
    else if (type == POKECENTER) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMON CENTER INTERFACE");

    // We're done with initialization
    refreshContext();

    // Only accepts </[ to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == '<' || ch == '[') break;
    }

    // If we got here, </[ has been pressed

    returnToParentContext();
}

Rect2D BuildingContext::getDimensions() {
    // Define dimensions of the new window
    Rect2D windowDimensions;
    windowDimensions.width = WINDOW_WIDTH - 6;
    windowDimensions.height = WINDOW_HEIGHT - 6;

    // Find the center of the parent window
    windowDimensions.x = (WINDOW_WIDTH - windowDimensions.width) / 2;
    windowDimensions.y = (WINDOW_HEIGHT - windowDimensions.height) / 2;

    return windowDimensions;
}