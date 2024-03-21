#include "contexts/ctx_world.h"
#include "entity/event.h"
#include "contexts/ctx_trainer_list.h"
#include "contexts/ctx_building.h"

PlayerEncounterScenario tryPlayerMovementInput(GameManager* game, int key) {
    switch (key) {
        case '7':
        case 'y': // PLAYER UP-LEFT
            return attemptMovePlayer(game, -1, -1);

        case '8':
        case 'k':
        case 'w':  // WASD alias
            // PLAYER UP
            return attemptMovePlayer(game, 0, -1);

        case '9':
        case 'u':
            // PLAYER UP-RIGHT
            return attemptMovePlayer(game, 1, -1);

        case '6':
        case 'l':
        case 'd':  // WASD alias
            // PLAYER RIGHT
            return attemptMovePlayer(game, 1, 0);

        case '3':
        case 'n':
            // PLAYER DOWN-RIGHT
            return attemptMovePlayer(game, 1, 1);

        case '2':
        case 'j':
        case 's':  // WASD alias
            // PLAYER DOWN
            return attemptMovePlayer(game, 0, 1);

        case '1':
        case 'b':
            // PLAYER DOWN-LEFT
            return attemptMovePlayer(game, -1, 1);

        case '4':
        case 'h':
        case 'a':  // WASD alias
            // PLAYER LEFT
            return attemptMovePlayer(game, -1, 0);

        default: return INVALID;
    }
}


bool worldContextInputHandler(GameManager* game, __attribute__((unused)) GameOptions* options, int key) {
    EntityManager* entManager = game->entManager;
    Player* player = game->player;

    // Try to see if the key was a player movement. If so, the player will have been moved, and we need to
    // handle the encounter event and return true, since the key was already caught.
    PlayerEncounterScenario scenario = tryPlayerMovementInput(game, key);
    if (scenario != INVALID) {
        dispatchPlayerEncounter(game, scenario);
        return true;
    }

    // Otherwise, try matching the key against the other cases
    switch (key) {
        case '5':
        case '.': {
            // REST
            Event* event = constructInputBlockingEvent(player->currentEntity, PLAYER_REST_TIME);
            event->resolveTime = game->time + event->cost;
            enqueueEvent(entManager, event);
            return true;
        }

        case 't': {
            // LIST TRAINERS
            Event* event = constructInputBlockingEvent(player->currentEntity, 1);
            event->resolveTime = game->time + event->cost;
            enqueueEvent(entManager, event);

            startTrainerListWindow(game);
            break;
        }

        case '>':
        case ']': {  // [ ] alias
            TileType type = game->world->current->tileset[player->mapY][player->mapX].type;
            if (type != POKECENTER && type != POKEMART) return false;

            Event* event = constructInputBlockingEvent(player->currentEntity, 1);
            event->resolveTime = game->time + event->cost;
            enqueueEvent(entManager, event);
            enterPlaceholderBuilding(game, type);
            break;
        }

        default: return false;
    }
    // We can only get here if it wasn't the no-op case, so we have handled the input
    return true;
}