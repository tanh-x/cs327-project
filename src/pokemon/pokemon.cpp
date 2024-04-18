#include <algorithm>
#include <random>
#include "utils/mathematics.hpp"
#include "pokemon/pokemon.hpp"
#include "utils/string_helpers.hpp"

#define MAX_POKEMON_LEVEL 100
#define SHINY_POKEMON_PROBABILITY 0.275f
#define GENERATE_IV() randomInt(0, 15)
#define STRUGGLE_MOVE_IDX 165
#define MAX_MOVES 4

Pokemon::Pokemon(PokemonDatabase* database, const std::shared_ptr<PokemonData> &pokemonData, int level) {
    this->data = pokemonData;

    // Generate base stats for the Pokemon
    this->baseMaxHp = data->statsTable.at(HP_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseAttack = data->statsTable.at(ATK_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseDefense = data->statsTable.at(DEF_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpecialAttack = data->statsTable.at(SPECIAL_ATK_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpecialDefense = data->statsTable.at(SPECIAL_DEF_STAT_IDX)->baseStat + GENERATE_IV();
    this->baseSpeed = data->statsTable.at(SPEED_STAT_IDX)->baseStat + GENERATE_IV();

    // Set the level and the stats for this level
    setPokemonLevel(level);
    this->health = maxHp;

    // Get the list of types of this pokemon
    for (const auto &typeRelation: pokemonData->typesTable) {
        this->types.insert(database->typeNameTable.at(typeRelation.second->typeId));
    }

    // Calculate the rest of the attributes of the Pokemon
    this->gender = randomInt(0, 1) == 0;
    this->isShiny = proba() < SHINY_POKEMON_PROBABILITY;

    // Compute the name of this pokemon
    this->name = (isShiny ? "Shiny " : "") + unkebabString(data->identifier);

    // Filter only moves available to our pokemon
    std::vector<std::pair<int, std::shared_ptr<PokemonMovesRelation>>> filteredMoves;
    std::copy_if(
        pokemonData->movesTable.begin(), pokemonData->movesTable.end(),
        std::back_inserter(filteredMoves),
        [level](std::pair<const int, std::shared_ptr<PokemonMovesRelation>> move) -> bool {
            return move.second->level <= level && move.second->pokemonMoveMethodId == 1;
        });

    if (filteredMoves.empty()) {
        // If no moves are available, give the Pokemon the move Struggle
        moveSet.push_back(database->movesTable.at(STRUGGLE_MOVE_IDX));
        return;
    }

    // Otherwise, check if there are 1 or 2 moves in the filtered vector
    if (filteredMoves.size() <= MAX_MOVES) {
        for (auto &moveRelation: filteredMoves) {
            int moveId = moveRelation.second->moveId;
            moveSet.push_back(database->movesTable.at(moveId));
        }
        return;
    }

    // Otherwise, shuffle and take the first two
    std::shuffle(filteredMoves.begin(), filteredMoves.end(), std::default_random_engine(time(nullptr)));

    // Then add it to the moveset
    for (int i = 0; i < MAX_MOVES; i++) {
        int moveId = filteredMoves[i].second->moveId;
        moveSet.push_back(database->movesTable.at(moveId));
    }
}

int Pokemon::leveledHp(int base, int level) {
    return (base * 2 * level / 100) + level + 10;
}

int Pokemon::leveledStat(int base, int level) {
    return (base * 2 * level / 100) + 5;
}

std::shared_ptr<Pokemon> Pokemon::generateWildPokemon(
    PokemonDatabase* database,
    int localManhattanDist,
    float localMenaceLevel
) {
    // Get a random Pokemon
    int idx = database->pokemonIds[randomInt(0, database->numPokemon - 1)];
    std::shared_ptr<PokemonData> data = database->pokemonTable.at(idx);

    // Compute its level based on distance and menace level
    int maxLevel = clamp(localManhattanDist / 2, 1, MAX_POKEMON_LEVEL);
    int minLevel = clamp((localManhattanDist - 200) / 2, 1, MAX_POKEMON_LEVEL);
    int level = randomInt(minLevel, maxLevel);
    level += static_cast<int>(localMenaceLevel * randomFloat(0.3f, 2.65f));
    level = clamp(level, minLevel, maxLevel);

    return std::make_shared<Pokemon>(database, data, level);
}

std::string Pokemon::statsString() const {
    std::ostringstream oss;
    oss << "ATK: " << attack << ", "
        << "DEF: " << defense << ", "
        << "SATK: " << specialAttack << ", "
        << "SDEF: " << specialDefense << ", "
        << "SPD: " << speed;

    return oss.str();
}

std::string Pokemon::movesString() const {
    std::ostringstream oss;
    oss << "Moves: ";

    for (const auto &move: moveSet) {
        if (move) oss << unkebabString(move->identifier) << " | ";
    }

    // Remove the trailing delimiter
    std::string result = oss.str();
    if (!moveSet.empty()) result = result.substr(0, result.size() - 3);

    return result;
}

std::string Pokemon::typesString() const {
    std::ostringstream oss;
    oss << "";

    for (const auto &typeNameData: types) {
        oss << unkebabString(typeNameData->name) << "/";
    }

    std::string result = oss.str();
    if (!types.empty()) result = result.substr(0, result.size() - 1);

    return result;
}

void Pokemon::setPokemonLevel(int newLevel) {
    this->level = clamp(newLevel, 0, MAX_POKEMON_LEVEL);

    // Compute the leveled up stats
    this->maxHp = leveledHp(baseMaxHp, level);
    this->attack = leveledStat(baseAttack, level);
    this->defense = leveledStat(baseDefense, level);
    this->specialAttack = leveledStat(baseSpecialAttack, level);
    this->specialDefense = leveledStat(baseSpecialDefense, level);
    this->speed = leveledStat(baseSpeed, level);
}

bool Pokemon::isDead() {
    return health <= 0;
}
