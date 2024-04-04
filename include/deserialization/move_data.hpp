#ifndef MOVE_DATA_HPP
#define MOVE_DATA_HPP

#include <string>
#include "deserializers.hpp"

#define MOVE_DATA_CSV_NAME "moves.csv"

class MoveData : public AbstractDeserializable {
public:
    int id;
    std::string identifier;
    int generationId;
    int typeId;
    int power;
    int pp;
    int accuracy;
    int priority;
    int targetId;
    int damageClassId;
    int effectId;
    int effectChance;
    int contestTypeId;
    int contestEffectId;
    int superContestEffectId;

    MoveData(
        int id,
        std::string identifier,
        int generationId,
        int typeId,
        int power,
        int pp,
        int accuracy,
        int priority,
        int targetId,
        int damageClassId,
        int effectId,
        int effectChance,
        int contestTypeId,
        int contestEffectId,
        int superContestEffectId
    ) : AbstractDeserializable(),
        id(id),
        identifier(std::move(identifier)),
        generationId(generationId),
        typeId(typeId),
        power(power),
        pp(pp),
        accuracy(accuracy),
        priority(priority),
        targetId(targetId),
        damageClassId(damageClassId),
        effectId(effectId),
        effectChance(effectChance),
        contestTypeId(contestTypeId),
        contestEffectId(contestEffectId),
        superContestEffectId(superContestEffectId) {}

    [[maybe_unused]] static MoveData* deserialize(const std::string &line);

    void printSelf() const override;
};


#endif
