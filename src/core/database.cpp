#include "core/database.hpp"
#include "deserialization/experience_data.hpp"
#include "deserialization/type_name_data.hpp"
#include "deserialization/pokemon_type_data.h"

// Initializes the database by parsing the .csv files and loading them into their corresponding
// deserialized data objects. "Tables" must be initialized in a top-down manner.
PokemonDatabase::PokemonDatabase() {
    // First, parse every CSV file into their own vectors
    // @formatter:off
    auto parsedPokemon       = loadFromCsv<PokemonData>(POKEMON_DATA_CSV_NAME, false);
    auto parsedStatsRelation = loadFromCsv<PokemonStatsRelation>(POKEMON_STATS_DATA_CSV_NAME, false);
    auto parsedMovesRelation = loadFromCsv<PokemonMovesRelation>(POKEMON_MOVES_DATA_CSV_NAME, false);
    auto parsedSpecies       = loadFromCsv<PokemonSpeciesData>(POKEMON_SPECIES_DATA_CSV_NAME, false);
    auto parsedMoves         = loadFromCsv<MovesData>(MOVE_DATA_CSV_NAME, false);
    auto parsedExperience    = loadFromCsv<ExperienceData>(EXPERIENCE_DATA_CSV_NAME, false);
    auto parsedTypes         = loadFromCsv<TypeNameData>(TYPE_NAME_DATA_CSV_NAME, false);
    auto parsedStats         = loadFromCsv<StatsData>(STATS_DATA_CSV_NAME, false);
    auto parsedTypeRelation  = loadFromCsv<PokemonTypeData>(POKEMON_TYPE_DATA_CSV_NAME, false);
    // @formatter:on


    // Load some parsed vectors into hashmaps/tables as lookup indices
    for (const auto &species: parsedSpecies) speciesTable[species->id] = species;
    for (const auto &stats: parsedStats) statsTable[stats->id] = stats;
    for (const auto &move: parsedMoves) movesTable[move->id] = move;

//    std::unordered_map<int, std::shared_ptr<PokemonMovesRelation>> movesRelationTable;
//    std::unordered_map<int, std::shared_ptr<PokemonStatsRelation>> statsRelationTable;

    // "Left join" the species table with the Pokemon table
    for (const std::shared_ptr<PokemonData> &pokemon: parsedPokemon) {
        // Put the pokemon into the table
        pokemonTable[pokemon->id] = pokemon;

        // Look up and get the species data from the index
        pokemon->speciesData = speciesTable[pokemon->speciesId];
    }

    for (const std::shared_ptr<PokemonStatsRelation> &statsRelation: parsedStatsRelation) {

    }

}
