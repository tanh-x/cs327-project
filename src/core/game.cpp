// This file has components that drive the game, or otherwise is a very centerpiece part of the project
#include <unistd.h>
#include <unordered_set>

#include "core/game.hpp"
#include "graphics/renderer.hpp"
#include "entities/event.hpp"
#include "core/input.hpp"
#include "entities/pathfinding.hpp"
#include "context/ctx_battle_view.hpp"
#include "context/components/animations.hpp"
#include "utils/string_helpers.hpp"
#include "entities/entity_types/pokemon_vessel.hpp"

#define INITIAL_LAUNCH_WINDOW_HEIGHT 12
#define INITIAL_LAUNCH_WINDOW_WIDTH 36
#define NUM_POKEMON_SELECTION 7
#define NUM_FIRST_POKEMON 3

#define BATTLE_INITIATION_COOLDOWN 24
#define NPC_BATTLE_INITIATION_PROBABILITY 0.5f

// The main game loop
void gameLoop() {
    // Pointers to essential components
    GAME.quit_game = false;
    World* world = GAME.world;
    Player* player = GAME.player;

    // Draw once
    renderGameUpdate();

    initialLaunchWindow("Choose your three Pokemon");

    Event* event;
    while (!GAME.quit_game) {
        Map* map = world->current;
        EntityManager* entManager = GAME.currentEntManager;

        // Note: the first event ever (resolveTime = 0) is always the Player's first event
        while ((event = static_cast<Event*>(heap_remove_min(entManager->eventQueue))) != nullptr) {
            // Time travelling is strictly prohibited
            entManager->eventTime = max(entManager->eventTime, event->resolveTime);

            CorporealEntity* actor = event->actor;

            // If it's a player event, break the event loop.
            if (actor->type == EntityEnum::PLAYER && event->type == PLAYER_INPUT_BLOCKING) break;

            // Delegate the event action to the entity manager
            resolveEvent(event);

            // If the entity is near the player, and they haven't been defeated, and there isn't an active cooldown
            if (actor->activeBattle
                && max(abs(actor->mapX - player->mapX), abs(actor->mapY - player->mapY)) <= 1
                && entManager->eventTime >= entManager->nextBattleInitiationTime
                // Only enter the battle_opponent with 50% probability
                && proba() < NPC_BATTLE_INITIATION_PROBABILITY) {
                // Render the game before entering battle_opponent
                renderGameUpdate();
                usleep(STD_SLOW_FRAME_DELAY);

                // Then enter the battle_opponent
                auto* battleCtx = new BattleViewContext(GAME.context, actor);
                battleCtx->start();
                // Blocking call until the battle_opponent is finished

                // Do another render when we're done
                clear();
                renderGameUpdate();

                // Update the next possible initiation time
                entManager->nextBattleInitiationTime = entManager->eventTime + BATTLE_INITIATION_COOLDOWN;
            }

            // Queue next event for this entity
            Event* newEvent = actor->constructEventOnTurn();
            entManager->enqueueEvent(newEvent);

            // Dispose the old event
            disposeEvent(event);
        }

        // If we got here, it must be the player's turn, so we await input and handle it

        // The input blocking event is no longer needed
        disposeEvent(event);

        // Keep calling handlePlayerInput until it returns true
        while (!handlePlayerInput());

        // TODO: bad
        if (event->cost > 0
            && map->tileset[player->mapY][player->mapX].type == TileType::TALL_GRASS
            && proba() < map->wildernessLevel) {
            // Start wild pokemon battle

            auto* pokemon = new PokemonVessel(Pokemon::generateWildPokemon(
                GAME.database,
                manhattanDist(map->globalX, map->globalY, 0, 0),
                map->menaceLevel
            ));

            // Render the game before entering battle_opponent
            renderGameUpdate();
            usleep(STD_SLOW_FRAME_DELAY);

            // Then enter the battle_opponent
            auto* battleCtx = new BattleViewContext(GAME.context, pokemon);
            battleCtx->start();
            // Blocking call until the battle_opponent is finished

            // Do another render when we're done
            clear();
            renderGameUpdate();
        }

        // Render the game after handling the player event
        renderGameUpdate();

        // Also, the distance field cache must be invalidated in case the player moved or took some other action
        invalidateMemoization((DistanceField**) map->memoizedDistanceFields);
    }
}

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(MapEntryProps entryProps) {
    clear();  // Clear the ncurses render

    Player* player = GAME.player;
    Map* map = GAME.world->current;

    // Set the player position to the specified position in the entryProps
    player->mapX = entryProps.playerSpawnX;
    player->mapY = entryProps.playerSpawnY;

    // Clear all distance fields on this map
    invalidateMemoization((DistanceField**) map->memoizedDistanceFields);

    // Generate a new entity manager if needed
    if (map->entityManager == nullptr) {
        map->entityManager = new EntityManager();
        int numTrainers = static_cast<int>(
            getBaseNumTrainers(map->wildernessLevel)
            + randomFloat(-1.5f, 0.825f)
        );
        if (getEccentricity(map->globalX, map->globalY) > SALIENCY_THRESHOLD) numTrainers += SALIENT_MAP_TRAINER_BONUS;
        map->entityManager->spawnTrainers(map, numTrainers);
    }

    // Load in the new map's entity manager
    EntityManager* entManager = map->entityManager;
    GAME.currentEntManager = entManager;

    // (Re-)initialize the event system
    entManager->reinitializeEventQueue();
    entManager->fillFirstTurns();

    // Set the player vessel entity position to the player's new position (set in move)
    player->possessEntity(entManager->vesselEntity, entManager);
}

// Moves the player to an adjacent map, and makes a call to setupGameOnMapLoad
// Returns the pointer to that map
void moveInWorldDirection(CardinalDir dir) {
    int worldSeed = GAME.world->worldSeed;
    int x = GAME.player->globalX;
    int y = GAME.player->globalY;
    int dx = 0;
    int dy = 0;
    int playerSpawnX;
    int playerSpawnY;

    // Calculates where to spawn the player, by considering the gate position.
    // The gate positions are given by the global hash function
    switch (dir) {
        case NORTH: dy = -1;
            playerSpawnX = hashWithMapCardinalDir(x, y, NORTH, worldSeed);
            playerSpawnY = MAP_HEIGHT - 2;
            break;

        case SOUTH: dy = 1;
            playerSpawnX = hashWithMapCardinalDir(x, y, SOUTH, worldSeed);
            playerSpawnY = 1;
            break;

        case WEST: dx = -1;
            playerSpawnX = MAP_WIDTH - 2;
            playerSpawnY = hashWithMapCardinalDir(x, y, WEST, worldSeed);
            break;

        case EAST: dx = 1;
            playerSpawnX = 1;
            playerSpawnY = hashWithMapCardinalDir(x, y, EAST, worldSeed);
            break;
    }

    // Declare the position of the player to line up with the gate
    MapEntryProps entryProps;
    entryProps.playerSpawnX = playerSpawnX;
    entryProps.playerSpawnY = playerSpawnY;

    // Move there, and check if it succeeded or not
    if (moveToMap(x + dx, y + dy, nullptr) != nullptr) {
        // Delegate the rest of the work to setupGameOnMapLoad
        setupGameOnMapLoad(entryProps);
    }
}

// Moves the player to a map at the specified parameters (globalX, globalY)
// Returns the pointer to that map
Map* moveToMap(int globalX, int globalY, MapEntryProps* entryProps) {
    // Get the new map at the specified coordinates
    Map* newMap = getMap(GAME.world, globalX, globalY, true);

    // Check if it's a valid map, if so then actually move there, otherwise a nullptr will be returned.
    if (newMap != nullptr) {
        GAME.player->globalX = globalX;
        GAME.player->globalY = globalY;
        GAME.world->current = newMap;

        if (entryProps != nullptr) {
            // Place the player entity on the western gate (arbitrary choice)
            entryProps->playerSpawnX = 2;
            entryProps->playerSpawnY = hashWithMapCardinalDir(
                globalX, globalY,
                WEST,
                GAME.world->worldSeed
            );
        }
    }

    return newMap;
}

void initialLaunchWindow(const char* prompt) {
    Rect2D rect = {
        (WINDOW_WIDTH - INITIAL_LAUNCH_WINDOW_WIDTH) / 2,
        (WINDOW_HEIGHT - INITIAL_LAUNCH_WINDOW_HEIGHT) / 2,
        INITIAL_LAUNCH_WINDOW_WIDTH,
        INITIAL_LAUNCH_WINDOW_HEIGHT
    };

    // Start with an animation
    verticalExpandAnimation(rect, INTERVAL_60FPS_MICROS);

    WINDOW* newWindow = newwin(rect.height, rect.width, rect.y, rect.x);
    keypad(newWindow, true);
    box(newWindow, 0, 0);

    wrefresh(newWindow);

    // Set up Pokemon selections
    PokemonDatabase* database = GAME.database;
    std::shared_ptr<PokemonData> pokemonSelections[NUM_POKEMON_SELECTION] = {
        database->pokemonTable.at(1),  // Bulbasaur
        database->pokemonTable.at(4),  // Charamander
        database->pokemonTable.at(7)   // Squirtle
    };

    // Generate some random Pokemon
    for (int i = 3; i < NUM_POKEMON_SELECTION; i++) {
        // First 200 Pokemon are probably more well-known, I dunno
        int idx = database->pokemonIds[randomInt(0, 150)];
        pokemonSelections[i] = database->pokemonTable.at(idx);
    }

    std::unordered_set<int> selections;

    int highlightedIdx = -1;

    mvwprintw(newWindow, 0, 3, "[ %s ]", prompt);

    while (true) {
        // Print out the pokemon and their name
        for (int line = 0; line < NUM_POKEMON_SELECTION; line++) {
            auto pokemonData = pokemonSelections[line];

            // Get the type name
            std::ostringstream oss;
            oss << "";
            for (const auto &typeRelation: pokemonData->typesTable) {
                std::shared_ptr<TypeNameData> typeName = database->typeNameTable.at(typeRelation.second->typeId);
                oss << unkebabString(typeName->name) << "/";
            }
            std::string typeName = oss.str();
            typeName = typeName.substr(0, typeName.size() - 1);

            // Form the string
            std::string lineStr;
            if (selections.empty() || selections.find(line) == selections.end()) {
                lineStr = (highlightedIdx == line ? "  > " : "    ")
                          + unkebabString(pokemonData->identifier)
                          + " (" + typeName + ") "
                          + (highlightedIdx == line ? "<" : "");
            } else {
                lineStr = "[+] " + unkebabString(pokemonData->identifier) + " (" + typeName + ") ";
            }

            mvwaddstr(
                newWindow, line + 1, 1,
                rightPad(lineStr, INITIAL_LAUNCH_WINDOW_WIDTH - 3).c_str()
            );
        }

        wrefresh(newWindow);

        int ch = getch();
        switch (ch) {
            case '\n':
            case ' ': {
                // Check if the highlightedIdx index is valid
                if (highlightedIdx < 0 || highlightedIdx >= NUM_POKEMON_SELECTION) continue;

                // Add the selection
                selections.insert(highlightedIdx);

                // If we don't have 3 pokemon yet, let the player choose more
                if (selections.size() < NUM_FIRST_POKEMON) continue;

                // Otherwise, add the 3 pokemon to the player's inventory
                for (int id: selections) {
                    // Add the Pokemon to the player's Pokemon inventory
                    auto newPokemon = std::make_shared<Pokemon>(database, pokemonSelections[id], 1);
                    GAME.pokemonInventory.push_back(newPokemon);
                }

                delwin(newWindow);
                renderGameUpdate();
                return;
            }

            case KEY_UP:
            case KEY_LEFT:
            case 'w':
            case 'a': {
                highlightedIdx = clamp(highlightedIdx - 1, 0, NUM_POKEMON_SELECTION - 1);
                continue;
            }
            case KEY_DOWN:
            case KEY_RIGHT:
            case 's':
            case 'd': {
                highlightedIdx = clamp(highlightedIdx + 1, 0, NUM_POKEMON_SELECTION - 1);
                continue;
            }
            default: {}
        }
    }
}
