#include "contexts/abstract_context.hpp"
#include "contexts/ctx_world.hpp"
#include "contexts/ctx_trainer_list.hpp"

AbstractContext::AbstractContext(ContextType type, AbstractContext* parent, Rect2D dimensions) {
    this->window = nullptr;
    this->type = type;
    this->parent = parent;
    this->dimensions = dimensions;
}

// Disposes of the context, and restores the terminal to the parent window.
// The caller must change the context of the game manager back to the parent context before this.
void AbstractContext::returnToParentContext() {
    if (parent != nullptr) {
        delwin(window);
        wrefresh(parent->window);
    }
    GAME.context
}

// Instantiates the child context as well as a constructed window.
//AbstractContext* AbstractContext::constructChildWindowContext(ContextType contextType, Rect2D rect) {
//    auto* ctx = new AbstractContext(contextType, this, rect);
//    ctx->constructWindow();
//    return ctx;
//}

// Constructs a basic popup window, and sets the current window to it
WINDOW* AbstractContext::constructWindow() {
    WINDOW* newWindow = newwin(dimensions.height, dimensions.width, dimensions.y, dimensions.x);

    // Initialize window stuff
    keypad(newWindow, true);

    // Initialize styling
    box(newWindow, 0, 0);

    // Set the context's pointer to it and return it as well
    this->window = newWindow;
    return newWindow;
}

void AbstractContext::refreshContext() {
    wrefresh(window);
}

bool emptyInputHandler(int key) {
    return false;
}

// TODO: refactor this into polymorphism
// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in the key from getch() to carry out the correct action
// with respect to the current game context; returning whether the input was caught. If not, the key will be
// propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(int key) {
    switch (type) {
        case ContextType::WORLD_CONTEXT: return worldContextInputHandler;
        case ContextType::TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return emptyInputHandler;
    }
}
