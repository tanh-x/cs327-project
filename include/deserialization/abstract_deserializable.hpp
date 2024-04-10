#ifndef PROJECT_ABSTRACT_DESERIALIZABLE
#define PROJECT_ABSTRACT_DESERIALIZABLE

#include <vector>
#include <memory>
#include <fstream>
#include "deserializers.hpp"

#define VERBOSE_CSV_PARSING true
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


template<typename T>
std::vector<std::unique_ptr<T>> loadFromCsv(const std::string &csvFileName) {
    // Assert the covariance property
    static_assert(
        std::is_base_of_v<AbstractDeserializable, T>,
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
