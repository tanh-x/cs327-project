#include <cmath>
#include <csignal>
#include "battle/battle_manager.hpp"
#include "utils/misc.hpp"
#include "context/components/elements.hpp"

#define AI_CHANGE_POKEMON_BASE_PROBABILITY 0.04f
#define AI_HIGHER_DMG_MOVE_EXP_PROBABILITY 0.72f
#define AI_HEURISTIC_STAB_FACTOR 1.77f
#define AI_POKEMON_CHANGE_COOLDOWN 2
#define SLEEP_BETWEEN_PLY_MILLIS 500

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

    // Otherwise, this is a valid move
    playerNextMoveType = TurnMoveType::ATTACK;
    playerNextAttack = move;
    return true;
}

bool BattleManager::submitPokemonChange(int newIdx) {
    // Check if index is in range
    if (newIdx < 0 || newIdx >= static_cast<int>(friendlyPokemon->size())) return false;

    // Check if Pokemon is already dead
    if (friendlyPokemon->at(newIdx)->isDead()) return false;

    // If every check passed, then queue a pokemon change action
    playerNextMoveType = TurnMoveType::CHANGE_POKEMON;
    activeFriendlyIdx = newIdx;
    return true;
}

void BattleManager::executeTurn() {
    // TODO: Delete this
    spaces(battleCtx, 1, 1, 50);
    spaces(battleCtx, 2, 1, 50);

    // First, let the AI take its turn
    computeOpponentTurnAI();

    // Then, determine priority
    int playerPriorityValue = playerNextMoveType == TurnMoveType::ATTACK ? playerNextAttack->priority : INT_MAX;
    int npcPriorityValue = npcNextMoveType == TurnMoveType::ATTACK ? npcNextAttack->priority : INT_MAX;

    bool playerPriority;
    // Compare current priority value
    if (playerPriorityValue == npcPriorityValue) {
        // Tiebreaker with Pokemon speed
        if (battleCtx->friendlyActive->speed == battleCtx->opponentActive->speed) {
            // If also tied, then randomize
            playerPriority = randomInt(0, 1) == 0;
        } else playerPriority = battleCtx->friendlyActive->speed > battleCtx->opponentActive->speed;
    } else playerPriority = playerPriorityValue > npcPriorityValue;


    // Execute turns in the corresponding order
    if (playerPriority) {
        executePlayerTurn();
        battleCtx->refreshContext();
        usleep(SLEEP_BETWEEN_PLY_MILLIS * 1000);
        executeOpponentTurn();
    } else {
        executeOpponentTurn();
        battleCtx->refreshContext();
        usleep(SLEEP_BETWEEN_PLY_MILLIS * 1000);
        executePlayerTurn();
    }
    battleCtx->refreshContext();

    // We're done, so clean up the turn queue holders
    playerNextMoveType = TurnMoveType::NONE;
    playerNextAttack = nullptr;
    npcNextMoveType = TurnMoveType::NONE;
    npcNextAttack = nullptr;

    // ...and increment the turn counter
    turn++;

}

void BattleManager::executePlayerTurn() {
    switch (playerNextMoveType) {
        case TurnMoveType::ATTACK: {
            bool miss = randomInt(0, 100) > playerNextAttack->accuracy;
            if (miss) {
                mvwprintw(battleCtx->window, 1, 1, "Missed");
                // TODO: Indicate miss
                return;
            }

            int damage = inflictDamage(battleCtx->friendlyActive, battleCtx->opponentActive, playerNextAttack);
            mvwprintw(battleCtx->window, 1, 1, "Dealt %d damage", damage);
            // TODO: Indicate this
            return;
        }

        case TurnMoveType::ITEM: {

        }

        case TurnMoveType::CHANGE_POKEMON: {
            battleCtx->friendlyActive = friendlyPokemon->at(activeFriendlyIdx);
            mvwprintw(
                battleCtx->window, 1, 1, "Changed pokemon to %s",
                battleCtx->friendlyActive->name.c_str()
            );
            // TODO: Indicate this
        }
        case TurnMoveType::NONE: return;
    }
}

void BattleManager::executeOpponentTurn() {
    switch (npcNextMoveType) {
        case TurnMoveType::ATTACK: {
            bool miss = randomInt(0, 100) > npcNextAttack->accuracy;
            if (miss) {
                mvwprintw(battleCtx->window, 2, 1, "Missed");
                // TODO: Indicate miss
                return;
            }

            int damage = inflictDamage(battleCtx->opponentActive, battleCtx->friendlyActive, npcNextAttack);
            mvwprintw(battleCtx->window, 2, 1, "Dealt %d damage", damage);
            // TODO: Indicate this
            return;
        }
        case TurnMoveType::ITEM: {

        }
        case TurnMoveType::CHANGE_POKEMON: {
            battleCtx->opponentActive = opponentPokemon->at(activeOpponentIdx);
            mvwprintw(
                battleCtx->window, 1, 1, "Opponent changed pokemon to %s",
                battleCtx->opponentActive->name.c_str()
            );
            // TODO: Indicate this
        }
        case TurnMoveType::NONE: return;
    }
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


void BattleManager::computeOpponentTurnAI() {
    std::shared_ptr<Pokemon> activePokemon = battleCtx->opponentActive;
    int n = static_cast<int>(opponentPokemon->size());

    // Incentivise the AI to change Pokemon if health is low
    float changePokemonProbaBonus = -0.4f * (float(activePokemon->health) / float(activePokemon->maxHp)) + 0.37f;

    if (proba() < AI_CHANGE_POKEMON_BASE_PROBABILITY + changePokemonProbaBonus
        && pokemonChangeCooldown <= 0) {
        // The AI has decided to change Pokemon

        int healthiestPokemonIdx = -1;
        int maxHealth = -1;

        // Find the Pokemon with the most health
        for (int i = 0; i < n; i++) {
            std::shared_ptr<Pokemon> pokemon = opponentPokemon->at(i);
            if (pokemon->isDead() || pokemon->health < maxHealth) continue;

            maxHealth = pokemon->health;
            healthiestPokemonIdx = i;
        }

        // All opponent Pokemon are dead
        if (healthiestPokemonIdx == -1) return;

        // Otherwise, switch to it
        npcNextMoveType = TurnMoveType::CHANGE_POKEMON;
        activeOpponentIdx = healthiestPokemonIdx;

        // Reset the pokemon change cooldown
        pokemonChangeCooldown = AI_POKEMON_CHANGE_COOLDOWN;

    } else {
        // The AI will use a normal move
        std::shared_ptr<MovesData> chosenMove = nullptr;
        float maxHeuristic = -INFINITY;

        // Go through every known move and pick the best one with some probability
        for (auto &move: activePokemon->moveSet) {
            auto heuristic = static_cast<float>(move->power);

            // If a type of the Pokemon matches the move's type, then multiply the heuristic by a factor
            for (const std::shared_ptr<TypeNameData> &type: activePokemon->types) {
                if (type->id == move->typeId) {
                    heuristic *= AI_HEURISTIC_STAB_FACTOR;
                    break;
                }
            }

            if (
                // If the heuristic value is less, also have a small chance of taking it
                (heuristic < maxHeuristic && proba() < (1 - AI_HIGHER_DMG_MOVE_EXP_PROBABILITY))
                // Otherwise, the heuristic value is higher, so take this move with some probability
                || proba() < AI_HIGHER_DMG_MOVE_EXP_PROBABILITY || chosenMove == nullptr) {

                // Then, choose this move as the current best move
                chosenMove = move;
                maxHeuristic = heuristic;
                continue;
            }
        }

        // Submit this move
        npcNextMoveType = TurnMoveType::ATTACK;
        npcNextAttack = chosenMove;

        // Decrement the pokemon change cooldown
        pokemonChangeCooldown--;
    }

}