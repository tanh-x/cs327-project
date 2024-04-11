#ifndef POKEMON_DATA_HPP
#define POKEMON_DATA_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include "abstract_deserializable.hpp"
#include "pokemon_moves_data.hpp"
#include "pokemon_stats_data.hpp"
#include "pokemon_species_data.hpp"

#define POKEMON_DATA_CSV_NAME "pokemon.csv"

class PokemonData : public AbstractDeserializable {
public:
    int id;
    std::string identifier;
    int speciesId;
    int height;
    int weight;
    int baseExperience;
    int order;
    bool isDefault;

    // Relationship to other data objects

    // Relation entity between the N:N relationship from Pokemon to Moves
    std::unordered_map<int, std::shared_ptr<PokemonMovesRelation>> movesTable {};

    // Relation entity between the N:N relationship from Pokemon to Stats
    std::unordered_map<int, std::shared_ptr<PokemonStatsRelation>> statsTable {};

    // N:1 relationship
    std::shared_ptr<PokemonSpeciesData> speciesData = nullptr;


    PokemonData(
        int id,
        std::string identifier,
        int speciesId,
        int height,
        int weight,
        int baseExperience,
        int order,
        bool isDefault
    ) : AbstractDeserializable(),
        id(id),
        identifier(std::move(identifier)),
        speciesId(speciesId),
        height(height),
        weight(weight),
        baseExperience(baseExperience),
        order(order),
        isDefault(isDefault) {}


    void printSelf() const override;

    [[maybe_unused]] static PokemonData* deserialize(const std::string &line);
};

#endif
