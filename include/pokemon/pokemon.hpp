#ifndef POKEMON_H
#define POKEMON_H

#include <utility>

#include "deserialization/pokemon_data.hpp"
#include "core/database.hpp"

class Pokemon {
public:
    std::shared_ptr<PokemonData> data;
    std::vector<std::shared_ptr<MovesData>> moveSet;
    std::string name() const;

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

    Pokemon(PokemonDatabase* database, const std::shared_ptr<PokemonData> &pokemonData, int level);

    // Calculates the max HP of the Pokemon at a given level
    static int leveledHp(int base, int level);

    // Calculates the stats of the Pokemon at a given level
    static int leveledStat(int base, int level);

    static std::shared_ptr<Pokemon> generateWildPokemon(
        PokemonDatabase* database,
        int localManhattanDist,
        float localMenaceLevel
    );

    std::string toString() const;
};

#endif
