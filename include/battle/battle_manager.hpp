#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include "entities/abstract_entity.hpp"
#include "context/ctx_battle_view.hpp"

#define MAX_DAMAGE 999
#define MIN_DAMAGE 1

class BattleViewContext;

enum class TurnMoveType {
    NONE,
    ATTACK,
    ITEM,
    CHANGE_POKEMON
};

// A manager object that takes in a BattleViewContext and handles its operations
class BattleManager {
public:
    int turn = 0;

    BattleManager(BattleViewContext* battleCtx);

    bool submitPokemonMove(std::shared_ptr<Pokemon> pokemon, std::shared_ptr<MovesData> move);

    bool submitPokemonChange(int newIdx);

    void executeTurn();

private:
    BattleViewContext* battleCtx;

    std::vector<std::shared_ptr<Pokemon>>* friendlyPokemon;
    std::vector<std::shared_ptr<Pokemon>>* opponentPokemon;

    // AI state values
    int pokemonChangeCooldown = 1;

    // Returns the damage amount that was actually inflicted
    static int inflictDamage(
        std::shared_ptr<Pokemon> attacker,
        std::shared_ptr<Pokemon> target,
        std::shared_ptr<MovesData> move
    );

    // Submitted turn actions (NOT THE CURRENT STATE OF THE GAME)

    int activeFriendlyIdx = 0;
    TurnMoveType playerNextMoveType = TurnMoveType::NONE;
    std::shared_ptr<MovesData> playerNextAttack = nullptr;

    int activeOpponentIdx = 0;
    TurnMoveType npcNextMoveType = TurnMoveType::NONE;
    std::shared_ptr<MovesData> npcNextAttack = nullptr;

    void executePlayerTurn();
    void executeOpponentTurn();

    void computeOpponentTurnAI();
};


#endif
