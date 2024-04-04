#ifndef POKEMON_MOVES_H
#define POKEMON_MOVES_H

#include <string>
#include "abstract_deserializable.hpp"

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

    void printSelf() const override;

    [[maybe_unused]] static PokemonMovesData* deserialize(const std::string &line);
};


#endif
