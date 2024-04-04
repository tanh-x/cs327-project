#ifndef STATS_DATA_H
#define STATS_DATA_H

#include <string>
#include "abstract_deserializable.hpp"

#define STATS_DATA_CSV_NAME "stats.csv"

class StatsData : public AbstractDeserializable {
public:
    int id;
    int damageClassId;
    std::string identifier;
    bool isBattleOnly;
    int gameIndex;

    StatsData(
        int id,
        int damageClassId,
        std::string  identifier,
        bool isBattleOnly,
        int gameIndex
    ) : AbstractDeserializable(),
        id(id),
        damageClassId(damageClassId),
        identifier(std::move(identifier)),
        isBattleOnly(isBattleOnly),
        gameIndex(gameIndex) {}

    void printSelf() const override;

    [[maybe_unused]] static StatsData* deserialize(const std::string &line);
};

#endif
