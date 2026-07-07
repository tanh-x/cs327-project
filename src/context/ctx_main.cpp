#include "context/ctx_main.hpp"
#include "entities/event.hpp"
#include "context/ctx_trainer_list.hpp"
#include "context/ctx_building.hpp"
#include "context/ctx_world_map.hpp"
#include "context/ctx_inventory.hpp"


// Instantiates the root context. Called once when initializing the game.
MainContext::MainContext() : AbstractContext(
    ContextType::MAIN_CONTEXT,
    nullptr,
    {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}
) {}

bool worldContextInputHandler(int key) {
    Player* player = GAME.player;

    // Try to see if the key was a player movement. If so, the player will have been moved, and we need to
    // handle the encounter event and return true, since the key was already caught.
    EncounterScenario scenario = tryPlayerMovementInput(player, key);
    if (scenario != EncounterScenario::INVALID) {
        dispatchPlayerEncounter(scenario);
        return true;
    }

    // Otherwise, try matching the key against the other cases
    switch (key) {
        case '5':
        case '.': {
            // REST
            enqueueInputBlockingEvent(PLAYER_REST_TIME);
            return true;
        }

        case 't': {
            // LIST TRAINERS
            enqueueInputBlockingEvent(0);
            auto* trainerListCtx = new TrainerListContext(
                dynamic_cast<MainContext*>(GAME.context),
                &GAME.currentEntManager->entities
            );
            trainerListCtx->start();
            // Blocking call until trainer list context completes its operations
            break;
        }

        case 'b':
        case 'B': {
            // INVENTORY
            enqueueInputBlockingEvent(0);
            auto* inventoryContext= new InventoryContext(GAME.context);
            inventoryContext->start();
            break;
        }

        case '>':
        case ']': {  // [ ] alias
            // Check if the player can enter a building
            TileType type = GAME.world->current->tileset[player->mapY][player->mapX].type;
            if (type != POKECENTER && type != POKEMART) return false;

            enqueueInputBlockingEvent(0);

            // Enter the building
            auto* buildingCtx = new BuildingContext(dynamic_cast<MainContext*>(GAME.context), type);
            buildingCtx->start();
            // Blocking call until the player exits the building
            break;
        }

        case 'm':
        case 'f': {
            // OPEN WORLD MAP
            enqueueInputBlockingEvent(0);
            auto* worldMapCtx = new WorldMapContext(GAME.context, GAME.world);
            worldMapCtx->start();
            // Blocking call until the map is exited, or if a fly operation was carried out
            break;
        }

        default: return false;
    }
    // We can only get here if it wasn't the no-op case, so we have handled the input
    return true;
}


EncounterScenario tryPlayerMovementInput(Player* player, int key) {
    switch (key) {
        case '7':
        case 'y': // PLAYER UP-LEFT
            return player->attemptMove(-1, -1);

        case '8':
        case 'k':
        case 'w':  // WASD alias
            // PLAYER UP
            return player->attemptMove(0, -1);

        case '9':
        case 'u':
            // PLAYER UP-RIGHT
            return player->attemptMove(1, -1);

        case '6':
        case 'l':
        case 'd':  // WASD alias
            // PLAYER RIGHT
            return player->attemptMove(1, 0);

        case '3':
        case 'n':
            // PLAYER DOWN-RIGHT
            return player->attemptMove(1, 1);

        case '2':
        case 'j':
        case 's':  // WASD alias
            // PLAYER DOWN
            return player->attemptMove(0, 1);

        case '1':
            // PLAYER DOWN-LEFT
            return player->attemptMove(-1, 1);

        case '4':
        case 'h':
        case 'a':  // WASD alias
            // PLAYER LEFT
            return player->attemptMove(-1, 0);

        default: return EncounterScenario::INVALID;
    }
}
