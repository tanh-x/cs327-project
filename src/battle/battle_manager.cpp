#include <cmath>
#include "battle/battle_manager.hpp"
#include "utils/misc.hpp"

BattleManager::BattleManager(BattleViewContext* battleCtx) {
    this->battleCtx = battleCtx;

    this->friendlyPokemon = &GAME.pokemonInventory;
    this->opponentPokemon = &battleCtx->opponent->pokemonInventory;
}

bool BattleManager::submitPokemonMove(
    std::shared_ptr<Pokemon> pokemon,
    std::shared_ptr<MovesData> move
) {
    // Dead pokemon can't do anything
    if (pokemon->isDead()) return false;

    // Check if the Pokemon knows the move
    if (!contains(pokemon->moveSet, move)) return false;

    return false;
}

bool BattleManager::submitPokemonChange(int newIdx) {
    // Check if index is in range
    if (newIdx < 0 || newIdx > friendlyPokemon->size()) return false;

    // Check if Pokemon is already dead
    if (friendlyPokemon->at(newIdx)->isDead()) return false;

    // If every check passed, then queue a pokemon change action
    playerNextMoveType = TurnMoveType::CHANGE_POKEMON;
    activeFriendlyIdx = newIdx;
    return true;
}

void BattleManager::executeTurn() {


    // We're done, so clean up the turn queue holders
    playerNextMoveType = TurnMoveType::NONE;
    playerNextAttack = nullptr;

    // ...and increment the turn counter
    turn++;
}

int BattleManager::inflictDamage(
    std::shared_ptr<Pokemon> attacker,
    std::shared_ptr<Pokemon> target,
    std::shared_ptr<MovesData> move
) {
    float base = 2.0f * static_cast<float>(attacker->level) / 5.0f + 2.0f;
    base *= static_cast<float>(move->power);
    base *= static_cast<float>(attacker->attack);
    base /= static_cast<float>(target->defense);
    base /= 50.0f;
    base += 2.0f;

    // A crit is hit when 0, 255 is less than basespeed / 2
    float criticalFactor = randomInt(0, 255) < (attacker->baseSpeed / 2) ? 1.5f : 1.0f;

    // Random is uniform from 85% to 100%
    float randomFactor = randomFloat(0.85, 1.0);

    // STAB is 1 by default...
    float stabFactor = 1.0f;
    for (const std::shared_ptr<TypeNameData> &type: attacker->types) {
        if (type->id != move->typeId) continue;

        // But if any of the attacker's type matches, then it's 1.5
        stabFactor = 1.5f;
        break;
    }

    // Put everything together
    int overall = static_cast<int>(round(base * criticalFactor * randomFactor * stabFactor));

    // Clamp it between reasonable values
    overall = clamp(overall, MIN_DAMAGE, MAX_DAMAGE);

    // Then, pass the calculated damage to the target
    int targetBeforeHp = target->health;
    target->sustainDamage(overall);
    int trueDamage = targetBeforeHp - target->health;

    // Return the amount of damage that was dealt
    return trueDamage;
}
