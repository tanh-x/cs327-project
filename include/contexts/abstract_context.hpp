#ifndef CONTEXT_FACADE_H
#define CONTEXT_FACADE_H
// Contexts are different stages of the game. The functionality of the game is different depending on whether
// we're in normal gameplay, a battle, or navigating a menu

#include <ncurses.h>
#include "context_type.hpp"
#include "utils/mathematics.hpp"

class AbstractContext {
public:
    WINDOW* window;
    ContextType type;

    // The main loop of the context UI
    virtual void start();

    // Instantiates a new context, but doesn't yet switch the game to it
    AbstractContext(ContextType type, AbstractContext* parent, Rect2D dimensions);

    // Disposes of the context, and restores the terminal to the parent window.
    // The caller must change the context of the game manager back to the parent context
    void returnToParentContext();

    // Calls wrefresh on the current window
    void refreshContext() const;

protected:
    AbstractContext* parent;
    Rect2D dimensions {};

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
