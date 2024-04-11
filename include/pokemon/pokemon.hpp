#ifndef POKEMON_H
#define POKEMON_H

#include <utility>

#include "deserialization/pokemon_data.hpp"

class Pokemon {
public:
    std::shared_ptr<PokemonData> data;

    int level;
    bool isShiny;
    bool gender;

//    Pokemon(std::shared_ptr<PokemonData> pokemonData) : data(std::move(pokemonData)) {};
    Pokemon(const std::shared_ptr<PokemonData> &pokemonData, int level);
private:
    // Pokemon base stats, with IV already added in
    int baseMaxHp;
    int baseAttack;
    int baseDefense;
    int baseSpecialAttack;
    int baseSpecialDefense;
    int baseSpeed;

    // Calculates the max HP of the Pokemon at a given level
    static int leveledHp(int base, int level);

    // Calculates the stats of the Pokemon at a given level
    static int leveledStat(int base, int level);
};

#endif
