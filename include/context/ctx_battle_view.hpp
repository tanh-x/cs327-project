#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include "core/game_manager.hpp"

#define DIALOG_WINDOW_HEIGHT 6

class BattleViewContext : public AbstractContext {
public:
    BattleViewContext(AbstractContext* parent, AbstractEntity* opponent);

    void start() override;

private:
    // The opponent we're fighting against
    AbstractEntity* opponent;

    // Pointer to the dialog window that will be created in the constructor
    WINDOW* dialogWindow;
};

#endif
