#ifndef EXPERIENCE_DATA_H
#define EXPERIENCE_DATA_H

#include "abstract_deserializable.hpp"

#define EXPERIENCE_DATA_CSV_NAME "experience.csv"

class ExperienceData : public AbstractDeserializable {
public:
    int growthRateId;
    int level;
    int experience;

    ExperienceData(
        int growthRateId,
        int level,
        int experience
    ) : AbstractDeserializable(),
        growthRateId(growthRateId),
        level(level),
        experience(experience) {}

    void printSelf() const override;

    [[maybe_unused]] static ExperienceData* deserialize(const std::string &line);
};


#endif
