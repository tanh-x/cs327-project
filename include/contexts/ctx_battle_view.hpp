#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include "core/game_manager.hpp"

#define DIALOG_WINDOW_HEIGHT 6

class BattleViewContext : public AbstractContext {
public:
    BattleViewContext(AbstractContext* parent, AbstractEntity* opponent);

    // The opponent we're fighting against
    AbstractEntity* opponent;

    void start() override;
};

#endif
