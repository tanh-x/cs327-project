#include <cstdlib>
#include "context/ctx_world_map.hpp"
#include "context/components/animations.hpp"
#include "context/components/elements.hpp"
#include "utils/string_helpers.hpp"

#define FOOTER_OFFSET 16
#define SALIENCY_THRESHOLD 243
#define PIVOT_SPEED 1
#define SIXTY_FOUR_SPACES "                                                                "

WorldMapContext::WorldMapContext(AbstractContext* parent, World* world) : AbstractContext(
    ContextType::WORLD_MAP_CONTEXT,
    parent,
    {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}
) {
    // Store the pointer to the world
    this->world = world;

    // Do a funny animation
    expandWindowVertical(dimensions, INTERVAL_60FPS_MICROS);

    // Construct the main window and switch to it
    constructWindow();

    // Add extra stuff
    windowTitle(this, "World Map", 2);
    horizontalSeparator(this, FOOTER_OFFSET);

    // Write the footer navigation guide
    mvwaddstr(
        window, dimensions.height - 1, 3,
        "[  wasd/arrow: Navigate  |  esc/f/m: Exit  |  enter/t: Fly  |  c: Focus  ]"
    );

    // We're done with initialization
}

void WorldMapContext::drawWorldMap(int pivotX, int pivotY) {
    int windowCenterX = dimensions.width / 2;
    int windowCenterY = FOOTER_OFFSET / 2;
    int spanX = dimensions.width - 2;
    int spanY = FOOTER_OFFSET;
    int scaleFactor = 3;

    for (int dx = 0; dx < spanX; dx += scaleFactor) {
        for (int dy = 1; dy < spanY; dy++) {
            // Calculate the global coordinate to draw here
            int mapX = pivotX + (dx - spanX / 2) / scaleFactor + 1;
            int mapY = pivotY + dy - spanY / 2;

            // Don't draw past the border
            if (abs(mapX) >= WORLD_X_SPAN || abs(mapY) >= WORLD_Y_SPAN) {
                mvwaddstr(
                    window,
                    windowCenterY + dy - spanY / 2,
                    windowCenterX + dx - spanX / 2,
                    "###"
                );

                continue;
            }

            // Get the map at this position
            Map* map = world->maps[mapY + WORLD_Y_SPAN][mapX + WORLD_X_SPAN];
            uint8_t mapEccentricity = world->eccentricity[mapY + WORLD_Y_SPAN][mapX + WORLD_X_SPAN];

            // Compute a bunch of bools to figure out what colorId and character to use
            bool isExplored = map != nullptr;
            bool isPivoted = mapX == pivotX && mapY == pivotY;
            bool isCurrent = mapX == world->current->globalX && mapY == world->current->globalY;

            int sparsityFactor = manhattan_dist(0, 0, 2 * mapX, mapY) / 2 + (mapEccentricity - 128) / 40;
            int colorId = max(30, (mapEccentricity % 6 + 36) - clamp(sparsityFactor, -2, 8));
            if (isPivoted) colorId = 22;            // 22 if pivoted (selected)
            else if (isExplored) colorId = 21;      // 21 if explored but not pivoted

            // Get the ncurses color pair
            int pair = COLOR_PAIR(colorId);

            // Find which character to use
            char tileChar = ' ';
            if (isCurrent) tileChar = '@';
            else if (mapX == 0 && mapY == 0) tileChar = 'O';
            else if (mapEccentricity > SALIENCY_THRESHOLD) tileChar = '?';

            // Format the 3-character string representing the map at this position
            char mapTileString[4];
            sprintf(mapTileString, "[%c]", tileChar);

            // Print it out
            wattron(window, pair);
            mvwaddstr(
                window,
                windowCenterY + dy - spanY / 2,
                windowCenterX + dx - spanX / 2,
                mapTileString
            );
            wattroff(window, pair);
        }
    }
}


void WorldMapContext::start() {
    AbstractContext::start();

    // Enter the main loop
    worldMapEntry();
    // worldMapEntry() only returns upon exit or we moved maps, so we destroy the window and the context

    // Swap the context back to the parent
    returnToParentContext();
    // We are done with the world map, so exit back to the call site
}

void WorldMapContext::worldMapEntry() {
    int pivotX = world->current->globalX;
    int pivotY = world->current->globalY;

    while (true) {
        // Coordinates are always 4 characters or less (3 digit, 1 sign)
        char xCoordString[5];
        sprintf(xCoordString, "%d", pivotX);
        leftPad(xCoordString, 4);
        char yCoordString[5];
        sprintf(yCoordString, "%d", pivotY);
        leftPad(yCoordString, 4);

        // Concatenate them
        char coordString[11];
        sprintf(coordString, "%s, %s", xCoordString, yCoordString);
        windowTitle(this, coordString, dimensions.width - 16);

        // Draw the world map
        drawWorldMap(pivotX, pivotY);

        Map* pivotedMap = world->maps[pivotY + WORLD_Y_SPAN][pivotX + WORLD_X_SPAN];
        // Clear the line
        mvwaddstr(window, FOOTER_OFFSET + 1, 2, SIXTY_FOUR_SPACES);
        // Write down the name of the map
        if (pivotedMap != nullptr) {
            mvwaddstr(window, FOOTER_OFFSET + 1, 2, pivotedMap->name.c_str());
        } else {
            mvwaddstr(window, FOOTER_OFFSET + 1, 2, "UNEXPLORED AREA");
        }


        refreshContext();
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
            case 'f':  // Toggle alias
            case 'm':  // Toggle alias
                return;  // Exit the loop

            case 'c': {
                // Recenter to player position
                pivotX = world->current->globalX;
                pivotY = world->current->globalY;
                continue;
            }

            case 't':
            case '\n': {
                // Fly to pivoted map

                // Check if the pivoted map is the current map
                if (pivotX == world->current->globalX && pivotY == world->current->globalY) continue;

                // Switch to that map
                MapEntryProps entryProps;
                moveToMap(pivotX, pivotY, &entryProps);
                setupGameOnMapLoad(entryProps);
                return;
            }

            case KEY_UP:
            case 'w': pivotY = clamp(pivotY - PIVOT_SPEED, -WORLD_Y_SPAN + 1, WORLD_Y_SPAN - 1);
                continue;

            case KEY_DOWN:
            case 's': pivotY = clamp(pivotY + PIVOT_SPEED, -WORLD_Y_SPAN + 1, WORLD_Y_SPAN - 1);
                continue;

            case KEY_LEFT:
            case 'a': pivotX = clamp(pivotX - PIVOT_SPEED, -WORLD_X_SPAN + 1, WORLD_X_SPAN - 1);
                continue;

            case KEY_RIGHT:
            case 'd': pivotX = clamp(pivotX + PIVOT_SPEED, -WORLD_X_SPAN + 1, WORLD_X_SPAN - 1);
                continue;

            default: {}
        }
    }
}
