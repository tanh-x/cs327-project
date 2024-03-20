#include <ncurses.h>
#include "core/input.h"

#define ESCAPE 27

// Returns whether the input was actually handled or not
bool handlePlayerInput(GameManager* game, GameOptions* options) {
    EntityManager* entManager = game->entManager;
    World* world = game->world;
    Map* map = world->current;
    Player* player = game->player;

    int ch = getch();
    switch (ch) {
        case '7':
        case 'y':
            // PLAYER UP-LEFT
            moveEntity(entManager, player->currentEntity, 1, -1);
            break;

        case '8':
        case 'k':
        case 'w':   // WASD alias
            // PLAYER UP
            break;

        case '9':
        case 'u':
            // PLAYER UP-RIGHT
            break;

        case '6':
        case 'l':
        case 'd':   // WASD alias
            // PLAYER RIGHT
            break;

        case '3':
        case 'n':
            // PLAYER DOWN-RIGHT
            break;

        case '2':
        case 'j':
        case 's':   // WASD alias
            // PLAYER DOWN
            break;

        case '1':
        case 'b':
            // PLAYER DOWN-LEFT
            break;

        case '4':
        case 'h':
        case 'a':   // WASD alias
            // PLAYER LEFT
            break;

//        case '>':
//        case ']':   // [ ] alias
//            // ENTER BUILDING
//            break;
//
//        case '<':
//        case '[':   // [ ] alias
//            // EXIT BUILDING
//            break;
//
//        case '5':
//        case '.':
//            // REST
//            break;
//
//        case 't':
//            break;
//            // LIST TRAINERS
//
//        case KEY_UP:
//            break;
//            // SCROLL UP
//
//        case KEY_DOWN:
//            break;
//            // SCROLL DOWN
//
//        case ESCAPE:
//            break;
//            // CLOSE POPUP

        case 'q':
        case 'Q':
            // QUIT GAME
            game->quit_game = true;
            break;


        default: return false;
    }
    // We can only get here if it wasn't the default case (no-op), so we have handled the input
    return true;
}