#ifndef POKEMON_MOVES_H
#define POKEMON_MOVES_H

#include <string>
#include "abstract_deserializable.hpp"
#include "moves_data.hpp"

#define POKEMON_MOVES_DATA_CSV_NAME "pokemon_moves.csv"

class PokemonMovesRelation : public AbstractDeserializable {
public:
    int pokemonId;
    int versionGroupId;
    int moveId;
    int pokemonMoveMethodId;
    int level;
    int order;

    // An entry from pokemon_moves.csv is a relation entity between a Pokemon and a move.
    // Hence, we store the pointer to the move that this relation is referring to.
    std::shared_ptr<MovesData> moveEntry = nullptr;

    PokemonMovesRelation(
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

    [[maybe_unused]] static PokemonMovesRelation* deserialize(const std::string &line);
};


#endif
