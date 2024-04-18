#include <cmath>
#include "context/components/elements.hpp"
#include "graphics/parse_frame.hpp"
#include "graphics/ncurses_artist.hpp"
#include "world/world.hpp"
#include "core/game.hpp"

void windowTitle(AbstractContext* context, const char* title, int x) {
    WINDOW* window = context->window;
    mvwprintw(window, 0, x, "[ %s ]", title);
}

void horizontalSeparator(AbstractContext* context, int x, int y, int width) {
    WINDOW* window = context->window;

    // Draw the separator
    wmove(window, y, x);

    // ACS_LTEE is '├'
    waddch(window, ACS_LTEE);
    for (int i = 0; i < width - 2; i++) {
        // ACS_HLINE is '─'
        waddch(window, ACS_HLINE);
    }

    // ACS_RTEE is '┤'
    waddch(window, ACS_RTEE);

    // Refresh the window
    wrefresh(window);
}

void verticalSeparator(AbstractContext* context, int x, int y, int height) {
    WINDOW* window = context->window;

    // Draw the separator
    wmove(window, y, x);

    // ACS_TTEE is '┬'
    waddch(window, ACS_TTEE);
    for (int i = 0; i < height - 2; i++) {
        // Move cursor down one line
        wmove(window, y + i + 1, x);

        // ACS_VLINE is '│'
        waddch(window, ACS_VLINE);
    }

    // Move cursor to the last position
    wmove(window, y + height - 1, x);

    // ACS_BTEE is '┴'
    waddch(window, ACS_BTEE);

    // Refresh the window
    wrefresh(window);
}


void spaces(AbstractContext* context, int x, int y, int width) {
    WINDOW* window = context->window;

    wmove(window, y, x);
    for (int i = 0; i < width; i++) {
        waddch(window, ' ');
    }

    // Refresh the window
    wrefresh(window);
}


void sequentialColoredBar(
    AbstractContext* context, int x, int y,
    int width, float value,
    int cmapPaletteOffset, int numPaletteColors,
    bool homogeneous,
    chtype filled, chtype empty
) {
    WINDOW* window = context->window;

    // Calculate the width of each color segment
    float segmentWidth = float(width) / float(numPaletteColors);
    int colorIndex;

    wmove(window, y, x);
    auto threshold = static_cast<int>(value);
    int lastIdx = clamp(std::floor(float(threshold) / segmentWidth), 0, numPaletteColors - 1);
    for (int i = 0; i < width; i++) {
        // Determine which color pair to use based on the current position
        colorIndex = clamp(std::floor(float(i) / segmentWidth), 0, numPaletteColors - 1);

        if (i < threshold) {
            int idx = (homogeneous ? lastIdx : colorIndex) + cmapPaletteOffset;
            wattron(window, COLOR_PAIR(idx));
            waddch(window, filled);
            wattroff(window, COLOR_PAIR(idx));
        } else {
            wattron(window, COLOR_PAIR(32));
            waddch(window, empty);
            wattroff(window, COLOR_PAIR(31));
        }
    }

    wrefresh(window);
}


void drawBox(
    WINDOW* window,
    int x, int y, int width, int height,
    chtype ls, chtype rs,
    chtype ts, chtype bs,
    chtype tl, chtype tr,
    chtype bl, chtype br
) {
    // Draw top and bottom horizontal lines
    mvwhline(window, y, x + 1, ts, width - 2);
    mvwhline(window, y + height - 1, x + 1, bs, width - 2);

    // Draw left and right vertical lines
    mvwvline(window, y + 1, x, ls, height - 2);
    mvwvline(window, y + 1, x + width - 1, rs, height - 2);

    // Place corners
    mvwaddch(window, y, x, tl);
    mvwaddch(window, y, x + width - 1, tr);
    mvwaddch(window, y + height - 1, x, bl);
    mvwaddch(window, y + height - 1, x + width - 1, br);
}


void rasterizePokemonSprite(WINDOW* window, int pokemonId, int x, int y, bool flip) {
    // If the Pokemon's id is more than 151, it is not a generation 1 pokemon, so we don't have a sprite
    // for it. Hence, take pokemon % 151 with 90% chance and use one of the three secret sprites with
    // 10% probability. This random is the same for every Pokemon of the same kind, and is determined
    // by world seed.
    if (pokemonId > 151) {
        int test = globalHashFunction(pokemonId, pokemonId ^ 0x14f2, GAME.world->worldSeed);
        pokemonId = (test % 10 == 0) ? ((pokemonId % 3) + 152) : ((pokemonId % 151) + 1);
    }
    Int2D atlasPosition = getAtlasCoordinate(pokemonId);

    for (int rasterY = 0; rasterY < SPRITE_HEIGHT; rasterY++) {
        for (int rasterX = 0; rasterX < SPRITE_HEIGHT; rasterX++) {
            // Compute the pixel position on the atlas texture image
            int atlasX = atlasPosition.x + (flip ? (SPRITE_HEIGHT - rasterX) : rasterX);
            int atlasY = atlasPosition.y + rasterY;

            // Get the pixel value at raster position
            auto value = spriteAtlas[atlasY][atlasX];
            int colorValue = PICO8_PALETTE_OFFSET + value / 16;

            // Draw the pixel
            wattron(window, COLOR_PAIR(colorValue));
            mvwaddstr(window, rasterY + y, ASPECT * rasterX + x, "##");
            wattroff(window, COLOR_PAIR(colorValue));
        }
    }
}
