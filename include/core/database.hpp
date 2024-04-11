#ifndef DATABASE_H
#define DATABASE_H


#include <unordered_map>
#include <memory>
#include "deserialization/pokemon_data.hpp"

class PokemonDatabase {
    // The main pokemon table
    std::unordered_map<int, std::shared_ptr<PokemonData>> pokemonTable;

    // Non-relation entity tables
    std::unordered_map<int, std::shared_ptr<PokemonSpeciesData>> speciesTable;
    std::unordered_map<int, std::shared_ptr<MovesData>> movesTable;
    std::unordered_map<int, std::shared_ptr<StatsData>> statsTable;

    // Initializes the database by parsing the .csv files and loading them into their corresponding
    // deserialized data objects. "Tables" must be initialized in a top-down manner.
    PokemonDatabase();
};

#endif
