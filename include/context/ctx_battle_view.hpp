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
#define POKEMON_RIGHT_Y 3

#define DIALOG_CHAR_PER_FRAME 3
#define DIALOG_FRAME_TIME 33333
#define DIALOG_WAIT_TIME 1000000

#define HEALTH_BAR_VERTICAL_MARGIN 2
#define HEALTH_BAR_HEIGHT 5
#define HEALTH_BAR_WIDTH 36

#define POKEMON_CAPTURE_PROBABILITY 0.4f

class BattleManager;

class BattleViewContext : public AbstractContext {
public:
    BattleManager* manager;

    bool battleTerminated = false;
    bool playerVictory = false;
    bool pokemonCaptured = false;

    std::shared_ptr<Pokemon> friendlyActive;
    std::shared_ptr<Pokemon> opponentActive;

    // The opponent we're fighting against
    AbstractEntity* opponent;

    BattleViewContext(AbstractContext* parent, AbstractEntity* opponent);

    void start() override;

    void onPokemonSelect(int idx);

    void writeToDialog(const std::vector<std::string>& lines, bool animate);

    void pokemonSwapAnimation(int oldId, int newId, bool left);

    void pokemonShakeAnimation(bool left);

    void renderHealthBar(bool friendly);

    void tryCapturePokemon();

private:
    // Pointer to the dialog window that will be created in the constructor
    WINDOW* dialogWindow;
    Rect2D dialogRect;

    Rect2D leftHealthBarRect;
    Rect2D rightHealthBarRect;

    void battleContextLoop();

    void renderWindowSkeleton();

    void renderPokemon();

    void pokemonEntryAnimation();

    void renderMoveList();
};

#endif
