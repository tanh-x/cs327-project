#ifndef POKEMON_STATS_DATA_H
#define POKEMON_STATS_DATA_H

#include <string>
#include "abstract_deserializable.hpp"
#include "stats_data.h"

#define POKEMON_STATS_DATA_CSV_NAME "pokemon_stats.csv"

class PokemonStatsRelation : public AbstractDeserializable {
public:
    int pokemonId;
    int statId;
    int baseStat;
    int effort;

    std::shared_ptr<StatsData> statsEntry = nullptr;

    PokemonStatsRelation(
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

    [[maybe_unused]] static PokemonStatsRelation* deserialize(const std::string &line);
};

#endif
