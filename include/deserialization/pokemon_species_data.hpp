#ifndef POKEMON_SPECIES_H
#define POKEMON_SPECIES_H

#include <string>
#include <utility>
#include "abstract_deserializable.hpp"

#define POKEMON_SPECIES_DATA_CSV_NAME "pokemon_species.csv"

class PokemonData;

class PokemonSpeciesData : public AbstractDeserializable {
public:
    int id;
    std::string identifier;
    int generationId;
    int evolvesFromSpeciesId;
    int evolutionChainId;
    int colorId;
    int shapeId;
    int habitatId;
    int genderRate;
    int captureRate;
    int baseHappiness;
    bool isBaby;
    int hatchCounter;
    bool hasGenderDifferences;
    int growthRateId;
    bool formsSwitchable;
    bool isLegendary;
    bool isMythical;
    int order;
    int conquestOrder;

    // It's helpful to maintain a list of Pokemon associated with this species
    std::vector<std::shared_ptr<PokemonData>> associatedPokemon {};

    PokemonSpeciesData(
        int id,
        std::string identifier,
        int generationId,
        int evolvesFromSpeciesId,
        int evolutionChainId,
        int colorId,
        int shapeId,
        int habitatId,
        int genderRate,
        int captureRate,
        int baseHappiness,
        bool isBaby,
        int hatchCounter,
        bool hasGenderDifferences,
        int growthRateId,
        bool formsSwitchable,
        bool isLegendary,
        bool isMythical,
        int order,
        int conquestOrder
    ) : AbstractDeserializable(),
        id(id),
        identifier(std::move(identifier)),
        generationId(generationId),
        evolvesFromSpeciesId(evolvesFromSpeciesId),
        evolutionChainId(evolutionChainId),
        colorId(colorId),
        shapeId(shapeId),
        habitatId(habitatId),
        genderRate(genderRate),
        captureRate(captureRate),
        baseHappiness(baseHappiness),
        isBaby(isBaby),
        hatchCounter(hatchCounter),
        hasGenderDifferences(hasGenderDifferences),
        growthRateId(growthRateId),
        formsSwitchable(formsSwitchable),
        isLegendary(isLegendary),
        isMythical(isMythical),
        order(order),
        conquestOrder(conquestOrder) {}

    void printSelf() const override;

    static PokemonSpeciesData* deserialize(const std::string &line);
};


#endif
