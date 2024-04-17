#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include "core/game_manager.hpp"

#define BATTLE_CTX_DIALOG_WINDOW_HEIGHT 6
#define BATTLE_CTX_FOOTER_SIZE 5
#define FOOTER_OFFSET (WINDOW_HEIGHT - BATTLE_CTX_FOOTER_SIZE - 1)
#define PROMPT_ACTIONS 4
#define PROMPT_ACTION_WIDTH 32

class BattleViewContext : public AbstractContext {
public:
    BattleViewContext(AbstractContext* parent, AbstractEntity* opponent);

    void start() override;

private:
    // The opponent we're fighting against
    AbstractEntity* opponent;

    // Pointer to the dialog window that will be created in the constructor
    WINDOW* dialogWindow;

    void battleContextLoop();
    void redrawWindow();
};

#endif
