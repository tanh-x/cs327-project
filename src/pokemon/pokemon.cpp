#include "utils/mathematics.hpp"
#include "pokemon/pokemon.hpp"

#define SHINY_POKEMON_PROBABILITY 0.21f
#define GENERATE_IV() randomInt(0, 15)

Pokemon::Pokemon(const std::shared_ptr<PokemonData> &pokemonData, int level) {
    this->data = pokemonData;

    // Generate base stats for the Pokemon
    this->baseMaxHp = data->statsTable.at(HP_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseAttack = data->statsTable.at(ATK_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseDefense = data->statsTable.at(DEF_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpecialAttack = data->statsTable.at(SPECIAL_ATK_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpecialDefense = data->statsTable.at(SPECIAL_DEF_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpeed = data->statsTable.at(SPEED_STAT_IDX)->baseStat + GENERATE_IV();

    // Calculate the rest of the attributes of the Pokemon
    this->level = level;
    this->gender = randomInt(0, 1) == 0;
    this->isShiny = proba() < SHINY_POKEMON_PROBABILITY;
}

int Pokemon::leveledHp(int base, int level) {
    return (base * 2 * level / 100) + level + 10;
}

int Pokemon::leveledStat(int base, int level) {
    return (base * 2 * level / 100) + 5;
}
