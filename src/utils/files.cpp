#include "utils/files.hpp"

#define POKEDEX_CSV_SUBPATH "pokedex/pokedex/data/csv/"

using namespace std::filesystem;

path findCsvFile(const std::string &filename) {
    path subpath = path(POKEDEX_CSV_SUBPATH);

    path path1 = path("/share/cs327/") / subpath / filename;
    printf("Searching in %s\n", path1.c_str());
    if (exists(path1)) return path1;

    path path2 = path(std::getenv("HOME")) / ".poke327/" / subpath / filename;
    printf("Searching in %s\n", path2.c_str());
    if (exists(path2)) return path2;

    printf("Error: File %s not found in any of the fallback locations\n", filename.c_str());
    exit(1);
}