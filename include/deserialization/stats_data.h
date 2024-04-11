#ifndef STATS_DATA_H
#define STATS_DATA_H

#include <string>
#include "abstract_deserializable.hpp"

#define STATS_DATA_CSV_NAME "stats.csv"
#define HP_STAT_IDX 1
#define ATK_STAT_IDX 2
#define DEF_STAT_IDX 3
#define SPECIAL_ATK_STAT_IDX 4
#define SPECIAL_DEF_STAT_IDX 5
#define SPEED_STAT_IDX 6

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
