#include "contexts/ctx_world.hpp"
#include "entity/event.hpp"
#include "contexts/ctx_trainer_list.hpp"
#include "contexts/ctx_building.hpp"


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
            startTrainerListWindow();
            break;
        }

        case '>':
        case ']': {  // [ ] alias
            TileType type = GAME.world->current->tileset[player->mapY][player->mapX].type;
            if (type != POKECENTER && type != POKEMART) return false;

            enqueueInputBlockingEvent(1);
            enterPlaceholderBuilding(type);
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
        case 'b':
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