#ifndef POKEMON_STATS_DATA_H
#define POKEMON_STATS_DATA_H

#include "deserializers.hpp"

#define POKEMON_STATS_DATA_CSV_NAME "pokemon_stats.csv"

class PokemonStatsData : public AbstractDeserializable {
public:
    int pokemonId;
    int statId;
    int baseStat;
    int effort;

    PokemonStatsData(
        int pokemonId,
        int statId,
        int baseStat,
        int effort
    ) : AbstractDeserializable(),
        pokemonId(pokemonId),
        statId(statId),
        baseStat(baseStat),
        effort(effort) {}

    void printSelf() const override;

    [[maybe_unused]] static PokemonStatsData* deserialize(const std::string &line);
};

#endif
