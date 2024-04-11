#include "deserialization/deserializers.hpp"
#include "deserialization/pokemon_data.hpp"
#include "deserialization/moves_data.hpp"
#include "deserialization/pokemon_moves_data.hpp"
#include "deserialization/experience_data.hpp"
#include "deserialization/pokemon_species_data.hpp"
#include "deserialization/type_name_data.hpp"
#include "deserialization/pokemon_stats_data.hpp"
#include "deserialization/stats_data.h"
#include "deserialization/pokemon_type_data.h"

void checkCsvParseFlag(const std::string &flag) {
    if (flag == "pokemon") {
        loadFromCsv<PokemonData>(POKEMON_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "movesTable") {
        loadFromCsv<MovesData>(MOVE_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "pokemon_moves") {
        loadFromCsv<PokemonMovesRelation>(POKEMON_MOVES_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "experience") {
        loadFromCsv<ExperienceData>(EXPERIENCE_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "pokemon_species") {
        loadFromCsv<PokemonSpeciesData>(POKEMON_SPECIES_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "type_names") {
        loadFromCsv<TypeNameData>(TYPE_NAME_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "pokemon_stats") {
        loadFromCsv<PokemonStatsRelation>(POKEMON_STATS_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "statsTable") {
        loadFromCsv<StatsData>(STATS_DATA_CSV_NAME, true);
        exit(0);
    } else if (flag == "pokemon_types") {
        loadFromCsv<PokemonTypeData>(POKEMON_TYPE_DATA_CSV_NAME, true);
        exit(0);
    }
    // Else, don't do anything
}

int toIntOrDefault(const std::string &s) {
    if (s.empty()) return EMPTY_INT;
    else return std::stoi(s);
}

std::string toStringOrDefault(int x) {
    if (x == EMPTY_INT) return STR_FOR_EMPTY_INT;
    else return std::to_string(x);
}

std::string toStringOrDefault(bool x) {
    return x ? "TRUE" : "FALSE";
}

bool parseToBool(const std::string &s) {
    return s == "1";
}
