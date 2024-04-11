#ifndef DATABASE_H
#define DATABASE_H


#include <unordered_map>
#include <memory>
#include "deserialization/pokemon_data.hpp"
#include "deserialization/pokemon_type_data.h"
#include "deserialization/type_name_data.hpp"

class PokemonDatabase {
public:
    // The main pokemon table
    std::unordered_map<int, std::shared_ptr<PokemonData>> pokemonTable;

    // A vector of added pokemon ids, for constant time random choice
    std::vector<int> pokemonIds;
    int numPokemon;

    // Non-relation entity tables
    std::unordered_map<int, std::shared_ptr<PokemonSpeciesData>> speciesTable;
    std::unordered_map<int, std::shared_ptr<PokemonTypeData>> typeRelationTable;
    std::unordered_map<int, std::shared_ptr<TypeNameData>> typeNameTable;
    std::unordered_map<int, std::shared_ptr<MovesData>> movesTable;
    std::unordered_map<int, std::shared_ptr<StatsData>> statsTable;

    // Initializes the database by parsing the .csv files and loading them into their corresponding
    // deserialized data objects. "Tables" must be initialized in a top-down manner.
    PokemonDatabase();
};

#endif
