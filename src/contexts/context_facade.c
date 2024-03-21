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

// Instantiates the child context, with a given constructed window.
Context* switchToChildWindowContext(Context* parent, ContextType type, WINDOW* window) {
    Context* ctx = malloc(sizeof(Context));
    ctx->window = window;
    ctx->type = type;
    ctx->parent = parent;

    GAME.context = ctx;
    wrefresh(window);
    return ctx;
}

// Constructs a basic popup window
WINDOW* constructWindow(int width, int height, int top, int left) {
    WINDOW* window = newwin(height, width, top, left);
    box(window, 0, 0);
    keypad(window, true);
    return window;
}

// Disposes of the context, and restores the terminal to the parent window.
// The caller must change the context of the game manager back to the parent context
void disposeContext(Context* ctx) {
    if (ctx->parent != NULL) {
        delwin(ctx->window);
        wrefresh(ctx->parent->window);
    }
    free(ctx);
}


bool (* dispatchContextInputHandler(ContextType type))(int key) {
    switch (type) {
        case WORLD_CONTEXT: return worldContextInputHandler;
        case TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return NULL;
    }
}
