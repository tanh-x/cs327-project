#ifndef CONTEXT_FACADE_H
#define CONTEXT_FACADE_H

#include <ncurses.h>
#include "context_enum.hpp"
#include "utils/mathematics.hpp"

// Contexts are different stages of the game. The functionality of the game is different depending on whether
// we're in normal gameplay, a battle, or navigating a menu. Contexts are invoked by a constructor call, which
// initializes the context. The caller MUST then invoke Context::start() to begin operation of the context window.
// Not calling Context::start() *may* result in undefined behaviour, as there can potentially be side effects that
// have already been made during the constructor call.
class AbstractContext {
public:
    WINDOW* window;
    ContextType type;
    Rect2D dimensions {};

    // Instantiates a new context, but doesn't yet switch the game to it
    AbstractContext(ContextType type, AbstractContext* parent, Rect2D dimensions);

    // The main loop of the context UI.
    // Every start() implementation MUST end with a singular returnToParentContext() call
    // When start() returns, the context is guaranteed to have been restored to the parent context. No additional
    // handling is necessary
    virtual void start();

    // Disposes of the context, which is assumed to be the current context, and restores the parent window.
    void returnToParentContext();

    // Calls wrefresh on the current window
    void refreshContext() const;

protected:
    AbstractContext* parent;

    // Constructs a basic popup window, and sets the current window to it
    WINDOW* constructWindow();
};

// TODO: old C code, refactor this into polymorphism
// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in the key from getch() to carry out the correct action
// with respect to the current game context; returning whether the input was caught. If not, the key will be
// propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(int key);

#endif
