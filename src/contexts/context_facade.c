#include <stdlib.h>
#include "contexts/context_facade.h"
#include "contexts/ctx_world.h"
#include "contexts/ctx_trainer_list.h"


// Instantiates the root context. Called once when initializing the game.
Context* instantiateRootContext() {
    Context* ctx = malloc(sizeof(Context));
    ctx->window = stdscr;
    ctx->type = WORLD_CONTEXT;
    ctx->parent = NULL;  // Root context has no parent

    return ctx;
}

// Instantiates the child context as well as a constructed window.
Context* constructChildWindowContext(ContextType type, Rect2D dimensions) {
    Context* ctx = malloc(sizeof(Context));
    ctx->window = constructWindow(dimensions);
    ctx->type = type;
    ctx->parent = GAME.context;
    ctx->dimensions = dimensions;

    GAME.context = ctx;
    return ctx;
}

// Constructs a basic popup window
WINDOW* constructWindow(Rect2D dimensions) {
    WINDOW* window = newwin(dimensions.height, dimensions.width, dimensions.y, dimensions.x);
    box(window, 0, 0);
    keypad(window, true);
    return window;
}

// Disposes of the context, and restores the terminal to the parent window.
// The caller must change the context of the game manager back to the parent context before this.
void returnToParentContext(Context* ctx) {
    if (ctx->parent != NULL) {
        delwin(ctx->window);
        wrefresh(ctx->parent->window);
    }
    free(ctx);
}

bool emptyInputHandler(int key) {
    return false;
}

// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in the key from getch() to carry out the correct action
// with respect to the current game context; returning whether the input was caught. If not, the key will be
// propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(int key) {
    switch (type) {
        case WORLD_CONTEXT: return worldContextInputHandler;
        case TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return emptyInputHandler;
    }
}
