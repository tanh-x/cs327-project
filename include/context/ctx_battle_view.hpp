#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include "core/game_manager.hpp"
#include "battle/battle_manager.hpp"

#define BATTLE_CTX_DIALOG_WINDOW_HEIGHT 6
#define BATTLE_CTX_FOOTER_SIZE 5
#define FOOTER_OFFSET (WINDOW_HEIGHT - BATTLE_CTX_FOOTER_SIZE - 1)
#define PROMPT_ACTIONS 4
#define PROMPT_ACTION_WIDTH 32

#define POKEMON_LEFT_X 3
#define POKEMON_LEFT_Y 13

#define POKEMON_RIGHT_X 80
#define POKEMON_RIGHT_Y 4

class BattleManager;

class BattleViewContext : public AbstractContext {
public:
    BattleManager* manager;

    std::shared_ptr<Pokemon> friendlyActive;
    std::shared_ptr<Pokemon> opponentActive;

    // The opponent we're fighting against
    AbstractEntity* opponent;

    BattleViewContext(AbstractContext* parent, AbstractEntity* opponent);

    void start() override;

private:

    // Pointer to the dialog window that will be created in the constructor
    WINDOW* dialogWindow;

    void battleContextLoop();

    void renderWindowSkeleton();

    void renderPokemon();

    void pokemonEntryAnimation();

    void renderMoveList();

    void onPokemonSelect()
};

#endif
