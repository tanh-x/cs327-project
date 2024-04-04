#ifndef POKEMON_MOVES_H
#define POKEMON_MOVES_H

#include <string>
#include "deserializers.hpp"

#define POKEMON_MOVES_DATA_CSV_NAME "pokemon_moves.csv"

class PokemonMovesData : public AbstractDeserializable {
public:
    int pokemonId;
    int versionGroupId;
    int moveId;
    int pokemonMoveMethodId;
    int level;
    int order;

    PokemonMovesData(
        int pokemonId,
        int versionGroupId,
        int moveId,
        int pokemonMoveMethodId,
        int level,
        int order
    ) : AbstractDeserializable(),
        pokemonId(pokemonId),
        versionGroupId(versionGroupId),
        moveId(moveId),
        pokemonMoveMethodId(pokemonMoveMethodId),
        level(level),
        order(order) {}

    // Deserializes a single line in the CSV file, and instantiates a new PokemonMovesData
    static PokemonMovesData* deserialize(const std::string &line);

    void printSelf() const override;
};


#endif
