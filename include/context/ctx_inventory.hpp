#ifndef INVENTORY_H
#define INVENTORY_H

#include <ncurses.h>
#include "core/game.hpp"
#include "context/ctx_main.hpp"
#include "context/abstract_context.hpp"

#define INVENTORY_WINDOW_WIDTH 64
#define SEPARATOR 32
#define INVENTORY_WINDOW_HEIGHT 10
#define NUM_INVENTORY_ENTRIES 3

class InventoryContext : public AbstractContext {
public:
    InventoryContext(AbstractContext* parent);

    void start() override;

private:
    bool inBattle;
    void inventoryEntry();
};


#endif
