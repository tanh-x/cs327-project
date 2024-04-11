#include <cstdlib>
#include <ctime>
#include "core/game_manager.hpp"
#include "graphics/renderer.hpp"
#include "graphics/artist.hpp"
#include "context/ctx_main.hpp"
#include "deserialization/deserializers.hpp"
#include "core/constants/text.hpp"

GameManager GAME;
GameOptions OPTIONS;

int main(int argc, char* argv[]) {
    // Default game option arguments
    bool doColoring = true;
    bool doBadApple = false;
    int numTrainers = 8;
    int frameTimeMicros = 1000000 / 60;

    printf(TITLE_POKE327_COLORED_GENERATED);
    printf("Loading game...\n\n");

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        auto flag = std::string(argv[i]);

        if (flag == "--nocolor") doColoring = false;
        else if (flag == "--badapple") doBadApple = true;
        else if (flag == "--numtrainers") numTrainers = (int) strtol(argv[i + 1], nullptr, 10);
        else if (flag == "--frametime") frameTimeMicros = (int) strtol(argv[i + 1], nullptr, 10);
        else checkCsvParseFlag(flag);
    }

    // Initialize options to reflect the parsed arguments
    OPTIONS.doBadApple = doBadApple;
    OPTIONS.doColoring = doColoring;
    OPTIONS.numTrainers = max(0, numTrainers);
    OPTIONS.frameTimeMicros = frameTimeMicros;

    // Get millisecond seed
    char* val = getenv("START");
    long long int invocationStartTime;
    if (val != nullptr) invocationStartTime = strtol(val, nullptr, 10) % 2190666 + 69820;
    else invocationStartTime = 0;
    struct timespec timeNano {};
    timespec_get(&timeNano, TIME_UTC);
    int timeSeedMilli = (int) ((timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime);


    // TODO: Main menu?

    // Initial set up
    GAME.currentEntManager = nullptr;
    GAME.quit_game = false;
    GAME.context = new MainContext();

    // Initialize the database
    GAME.database = new PokemonDatabase();

    // Create a player
    Player player;
    player.globalX = 0;
    player.globalY = 0;
    GAME.player = &player;

    // Initialize a world with all empty maps
    World world;
    initializeWorld(&world, timeSeedMilli);
    GAME.world = &world;

    // Get and initialize the central map
    Map* map = getMap(&world, player.globalX, player.globalY, false);

    // Set the current map to this map, allowing for convenient future reference
    world.current = map;

    // Initialize the memoized distance fields array
    for (auto &distanceField: map->memoizedDistanceFields) distanceField = nullptr;

    // spawnMap will allow for special case world generation
    map->isSpawnMap = true;

    // Overrides for the center map
    map->mapSeed = timeSeedMilli; // NOLINT(*-narrowing-conversions)
    map->name = "PALLET TOWN";
    map->wildernessLevel = 0.1025;
    map->menaceLevel = 10;

    // Generate the first map outside the game loop, prevents it from regenerating in the game loop.
    MapEntryProps entryProps = map->generateTerrain(world.worldSeed);

    // Also manually call the setup function on the spawnNpc map instead of doing it in the game loop
    setupGameOnMapLoad(entryProps);

    // Override game loop if using bad apple, DON'T PORT TO NCURSES!
//    if (doBadApple) {
//        printf(CLEAR_SCREEN);
//        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
//        worldToString(mapStr);
//        prettyPrint(mapStr, OPTIONS.doColoring);
//        return 0;
//    }

    // Initialize ncurses
    initializeRenderer(OPTIONS.doColoring);

    // Enter game loop
    gameLoop();

    printf("Shutting down game...");

    // Clean up
    GAME.currentEntManager = nullptr;
    free(GAME.context);
    GAME.context = nullptr;
    disposeWorld(&world);
    cleanUpRenderer();
    return 0;
}
