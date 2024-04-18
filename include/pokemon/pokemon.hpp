#ifndef POKEMON_H
#define POKEMON_H

#include <utility>
#include <unordered_set>

#include "deserialization/pokemon_data.hpp"
#include "core/database.hpp"
#include "utils/mathematics.hpp"

#define MAX_POKEMON_LEVEL 100

class Pokemon {
public:
    std::shared_ptr<PokemonData> data;
    std::vector<std::shared_ptr<MovesData>> moveSet;

    std::string name;

    int level;
    bool isShiny;
    bool gender;

    // Pokemon base stats, with IV already added in
    int baseMaxHp;
    int baseAttack;
    int baseDefense;
    int baseSpecialAttack;
    int baseSpecialDefense;
    int baseSpeed;

    // Leveled stats
    int maxHp;
    int attack;
    int defense;
    int specialAttack;
    int specialDefense;
    int speed;

    // Current HP of the Pokemon
    int health;
    bool isDead();

    // Set of type name pointers
    std::unordered_set<std::shared_ptr<TypeNameData>> types {};

    Pokemon(PokemonDatabase* database, const std::shared_ptr<PokemonData> &pokemonData, int level);

    // Informational methods
    [[nodiscard]] std::string statsString() const;

    [[nodiscard]] std::string movesString() const;

    [[nodiscard]] std::string typesString() const;

    // Battle methods

    // Sustains a damage value after the calculation is already mediated in BattleManager
    void sustainDamage(int damage);

    // Heals the Pokemon up to the max HP
    void heal(int amount);

    static std::shared_ptr<Pokemon> generateWildPokemon(
        PokemonDatabase* database,
        int localManhattanDist,
        float localMenaceLevel
    );

private:
    // Set the newLevel and the computed stat values for the pokemon
    void setPokemonLevel(int newLevel);

    // Calculates the max HP of the Pokemon at a given level
    static int leveledHp(int base, int level);

    // Calculates the stats of the Pokemon at a given level
    static int leveledStat(int base, int level);
};

#endif
