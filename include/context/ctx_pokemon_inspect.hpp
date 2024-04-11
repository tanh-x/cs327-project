#include <vector>
#include <memory>
#include "abstract_context.hpp"
#include "pokemon/pokemon.hpp"

#ifndef CTX_POKEMON_INSPECT_H

class PokemonInspectContext : public AbstractContext {
public:
    PokemonInspectContext(
        AbstractContext* parent,
        std::vector<std::shared_ptr<Pokemon>> friendlyPokemon,
        std::vector<std::shared_ptr<Pokemon>> opponentPokemon
    );

    void start() override;

private:
    std::vector<std::shared_ptr<Pokemon>> friendlyPokemon;
    std::vector<std::shared_ptr<Pokemon>> opponentPokemon;
    bool hasOpponent;
    int maxScroll;
    int opponentOffset;

    void pokemonListEntry();
};


#define CTX_POKEMON_INSPECT_H

#endif
