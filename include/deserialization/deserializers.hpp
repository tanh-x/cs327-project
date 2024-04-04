#ifndef DESERIALIZERS_HPP
#define DESERIALIZERS_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <climits>
#include <fstream>

#define VERBOSE_CSV_PARSING true
#define EMPTY_INT (INT_MAX)
#define STR_FOR_EMPTY_INT ("-")
#define COLUMN_SEPARATOR "\t| "
#define MAX_STRING_COLUMN_WIDTH 20

class AbstractDeserializable {
public:
    virtual void printSelf() const = 0;

    // Must also have a [static T deserialize(const string&)] method
    ~AbstractDeserializable() = default;

protected:
    AbstractDeserializable() = default;
};


std::filesystem::path findCsvFile(const std::string &filename);

void checkCsvParseFlag(const std::string &flag);

int toIntOrDefault(const std::string &s);

std::string toStringOrDefault(int x);

std::string toStringOrDefault(bool x);

bool parseToBool(const std::string &s);

template<typename T>
std::vector<std::unique_ptr<T>> loadFromCsv(const std::string &csvFileName) {
    // Assert the covariance property
    static_assert(
        std::is_base_of<AbstractDeserializable, T>::value,
        "T must be a subtype of AbstractDeserializable"
    );

    // Find the corresponding .csv file
    auto file = std::ifstream(findCsvFile(csvFileName));
    std::vector<std::unique_ptr<T>> result;

    // Get the first line of the CSV, which is the header line
    std::string line;
    getline(file, line);
    if (VERBOSE_CSV_PARSING) printf("\n\n");  // NOLINT

    // Read through every line in the .csv
    while (getline(file, line)) {
        // Deserialize the line into a newly instantiated object
        T* entry = T::deserialize(line);

        // Some lines might be skipped, in which case a nullptr is returned
        if (entry == nullptr) continue;

        // Add the object to the result vector
        result.push_back(std::unique_ptr<T>(entry));

        // Also print it out if specified
        if (VERBOSE_CSV_PARSING) entry->printSelf();  // NOLINT
    }

    return result;
}

#endif
