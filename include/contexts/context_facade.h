#ifndef CONTEXT_FACADE_H
#define CONTEXT_FACADE_H
// Contexts are different stages of the game. The functionality of the game is different depending on whether
// we're in normal gameplay, a battle, or navigating a menu

#include <ncurses.h>
#include "context_type.h"
#include "utils/mathematics.h"

typedef struct Context Context;
typedef struct Context {
    WINDOW* window;
    ContextType type;
    Context* parent;
    Rect2D dimensions;
} Context;


// Instantiates the root context. Called once when initializing the game.
Context* instantiateRootContext();

// Instantiates the child context as well as a constructed window.
Context* constructChildWindowContext(ContextType type, Rect2D dimensions);

// Constructs a basic popup window
WINDOW* constructWindow(Rect2D dimensions);

// Disposes of the context, and restores the terminal to the parent window.
// The caller must change the context of the game manager back to the parent context
void returnToParentContext(Context* ctx);

// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in the key from getch() to carry out the correct action
// with respect to the current game context; returning whether the input was caught. If not, the key will be
// propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(int key);


#endif
