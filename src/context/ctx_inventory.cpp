#include "context/ctx_inventory.hpp"
#include "context/components/animations.hpp"
#include "context/components/elements.hpp"
#include "context/ctx_battle_view.hpp"

InventoryContext::InventoryContext(AbstractContext* parent) : AbstractContext(
    ContextType::INVENTORY_CONTEXT,
    parent,
    {
        (WINDOW_WIDTH - INVENTORY_WINDOW_WIDTH) / 2,
        (WINDOW_HEIGHT - INVENTORY_WINDOW_HEIGHT) / 2,
        INVENTORY_WINDOW_WIDTH,
        INVENTORY_WINDOW_HEIGHT
    }
) {
    this->inBattle = parent->type == ContextType::BATTLE_CONTEXT;

    verticalExpandAnimation(dimensions, INTERVAL_60FPS_MICROS);

    constructWindow(true);

    windowTitle(this, "Inventory", 2);

    verticalSeparator(this, SEPARATOR, 0, dimensions.height);
}

void InventoryContext::start() {
    AbstractContext::start();

    inventoryEntry();

    returnToParentContext();
}

void InventoryContext::inventoryEntry() {
    int itemScroll = 0;
    int targetScroll = 0;
    bool leftScroll = true;

    std::vector<std::shared_ptr<Pokemon>> pokemonInventory = GAME.pokemonInventory;
    int numTargets = static_cast<int>(pokemonInventory.size()) + 1;

    const char* labels[NUM_INVENTORY_ENTRIES] = {
        "Health Potion",
        "Revive Pokemon",
        "Pokeball"
    };

    const int* counts[NUM_INVENTORY_ENTRIES] = {
        &GAME.player->numPotions,
        &GAME.player->numRevives,
        &GAME.player->numPokeballs
    };

    while (true) {
        for (int line = 0; line < NUM_INVENTORY_ENTRIES; line++) {
            int count = *counts[line];
            mvwprintw(
                window, line + 1, 1, " %s%s x%d%s  ",
                (itemScroll == line ? "> " : "  "),
                labels[line],
                count,
                (itemScroll == line) ? (leftScroll ? " <   " : " >>>>") : "     "
            );
        }


        if (!leftScroll) {
            for (int line = 0; line < numTargets; line++) {
                mvwprintw(
                    window, line + 1, SEPARATOR + 1, " %s%s%s   ",
                    (targetScroll == line ? "> " : "  "),
                    line == 0 ? "[CANCEL]" : pokemonInventory[line - 1]->name.c_str(),
                    (targetScroll == line ? " <" : "  ")
                );
            }
        }

        // We're done, so refresh
        refreshContext();

        // Listen for input
        int ch = getch();
        switch (ch) {
            case ESCAPE_KEY:
            case '`':  // Near-esc alias
            case '~':  // Near-esc alias
                return;  // Exit the loop

            case '\n':
            case ' ': {
                if (leftScroll) {
                    // Ran out of items
                    if (*counts[itemScroll] <= 0) continue;

                    if (itemScroll != 2) {
                        leftScroll = false;
                        continue;
                    }

                    // Else, try to capture pokemon

                    // Check if in battle
                    if (!inBattle) continue;

                    // Safe cast if inBattle
                    ((BattleViewContext*) parent)->tryCapturePokemon();

                    return;
                }

                if (targetScroll == 0) {
                    leftScroll = true;
                    continue;
                }
                std::shared_ptr<Pokemon> target = pokemonInventory[targetScroll - 1];

                if (itemScroll == 0) { // Potion
                    if (GAME.player->numRevives <= 0) continue;
                    if (target->health == target->maxHp || target->isDead()) continue;
                    target->heal(20);
                    GAME.player->numPotions--;
                    if (inBattle) {
                        auto* battleCtx = dynamic_cast<BattleViewContext*>(parent);
                        battleCtx->renderHealthBar(true);
                        battleCtx->writeToDialog({"You used a health potion on " + target->name + "."}, true);
                        battleCtx->manager->submitItemTurn();
                        battleCtx->manager->executeTurn();
                    }
                } else if (itemScroll == 1) {
                    if (!target->isDead() || GAME.player->numRevives <= 0) continue;
                    target->health = target->maxHp / 2;
                    GAME.player->numRevives--;
                    if (inBattle) {
                        auto* battleCtx = dynamic_cast<BattleViewContext*>(parent);
                        battleCtx->writeToDialog({"You revived " + target->name + "."}, true);
                        battleCtx->manager->submitItemTurn();
                        battleCtx->manager->executeTurn();
                    }
                }

                return;

            }

            case '4':
            case 'A':
            case 'a':
            case KEY_LEFT:leftScroll = true;
                continue;


            case '6':
            case 'D':
            case 'd':
            case KEY_RIGHT:leftScroll = false;
                continue;

            case '8':
            case 'W':
            case 'w':
            case KEY_UP: {
                if (leftScroll) {
                    itemScroll = clamp(itemScroll - 1, 0, NUM_INVENTORY_ENTRIES - 1);
                } else {
                    targetScroll = clamp(targetScroll - 1, 0, numTargets - 1);
                }
                continue;
            }

            case '2':
            case 'S':
            case 's':
            case KEY_DOWN: {
                if (leftScroll) {
                    itemScroll = clamp(itemScroll + 1, 0, NUM_INVENTORY_ENTRIES - 1);
                } else {
                    targetScroll = clamp(targetScroll + 1, 0, numTargets - 1);
                }
                continue;
            }
            default: {}
        }
    }
}
