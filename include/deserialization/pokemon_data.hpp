#ifndef POKEMON_DATA_HPP
#define POKEMON_DATA_HPP

#include <string>
#include "abstract_deserializable.hpp"

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
