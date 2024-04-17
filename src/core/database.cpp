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
    auto parsedTypeRelation  = loadFromCsv<PokemonTypeRelation>(POKEMON_TYPE_DATA_CSV_NAME, false);
    // @formatter:on


    // Load some parsed vectors into hashmaps/tables as lookup indices
    for (const auto &stats: parsedStats) statsTable[stats->id] = stats;
    for (const auto &move: parsedMoves) movesTable[move->id] = move;
    for (const auto &species: parsedSpecies) speciesTable[species->id] = species;
    for (const auto &type: parsedTypeRelation) typeRelationTable[type->pokemonId] = type;
    for (const auto &typeName: parsedTypes) typeNameTable[typeName->id] = typeName;

    // "Left join" the species table onto the Pokemon table
    for (const std::shared_ptr<PokemonData> &pokemon: parsedPokemon) {
        // Put the pokemon into the table
        pokemonTable[pokemon->id] = pokemon;

        // ...and the vector
        pokemonIds.push_back(pokemon->id);

        // Look up and get the species data from the index
        pokemon->speciesData = speciesTable.at(pokemon->speciesId);

        // Also handle the transpose association
        pokemon->speciesData->associatedPokemon.push_back(pokemon);
    }

    // "Left join" the stats table onto the Pokemon table
    for (const std::shared_ptr<PokemonStatsRelation> &statsRelation: parsedStatsRelation) {
        std::shared_ptr<PokemonData> pokemon = pokemonTable.at(statsRelation->pokemonId);
        pokemon->statsTable[statsRelation->statId] = statsRelation;
    }

    // "Left join" the types table onto the Pokemon table
    for (const std::shared_ptr<PokemonTypeRelation> &typeRelation: parsedTypeRelation) {
        std::shared_ptr<PokemonData> pokemon = pokemonTable.at(typeRelation->pokemonId);
        pokemon->typesTable[typeRelation->typeId] = typeRelation;
    }

    // "Left join" the moves table onto the Pokemon table, matching on pokemon.species_id
    for (const std::shared_ptr<PokemonMovesRelation> &moveRelation: parsedMovesRelation) {
        // Get all associated Pokemon from the transpose association map
        try {
            std::shared_ptr<PokemonSpeciesData> species = speciesTable.at(moveRelation->pokemonId);
            // Add this move to all of those Pokemon's move table
            for (const std::shared_ptr<PokemonData> &pokemon: species->associatedPokemon) {
                pokemon->movesTable[moveRelation->moveId] = moveRelation;
            }
        } catch (const std::out_of_range &unused) {
            // Fallback if unable to match with species table
            std::shared_ptr<PokemonData> pokemon = pokemonTable.at(moveRelation->pokemonId);
            pokemon->movesTable[moveRelation->moveId] = moveRelation;
        }
    }

    numPokemon = static_cast<int>(pokemonIds.size());

    // We're probably done here
}
