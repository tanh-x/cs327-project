#ifndef TYPE_NAME_DATA_H
#define TYPE_NAME_DATA_H

#include <utility>

#include "deserializers.hpp"

#define TYPE_NAME_DATA_CSV_NAME "type_names.csv"

class TypeNameData : public AbstractDeserializable {
public:
    int id;
    std::string name;

    TypeNameData(int id, std::string name) : AbstractDeserializable(), id(id), name(std::move(name)) {}

    void printSelf() const override;

    [[maybe_unused]] static TypeNameData* deserialize(const std::string &line);
};

#endif
