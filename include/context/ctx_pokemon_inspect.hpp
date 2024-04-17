#include <vector>
#include <memory>
#include "abstract_context.hpp"
#include "pokemon/pokemon.hpp"

#ifndef CTX_POKEMON_INSPECT_H

class PokemonInspectContext : public AbstractContext {
public:
    PokemonInspectContext(
        AbstractContext* parent,
        const std::vector<std::shared_ptr<Pokemon>> &pokemonList
    );

    PokemonInspectContext(
        AbstractContext* parent,
        const std::vector<std::shared_ptr<Pokemon>> &friendlyPokemon,
        const std::vector<std::shared_ptr<Pokemon>> &opponentPokemon
    );

    void start() override;

private:
    std::vector<std::shared_ptr<Pokemon>> pokemonList;
    std::vector<std::shared_ptr<Pokemon>> opponentPokemon;
    int numEntries;

    void pokemonListEntry();
};


#define CTX_POKEMON_INSPECT_H

#endif
