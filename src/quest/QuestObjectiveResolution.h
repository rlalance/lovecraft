#ifndef QUEST_OBJECTIVE_RESOLUTION_H
#define QUEST_OBJECTIVE_RESOLUTION_H

#include <rapidjson/document.h>
#include <utility/YiString.h>
#include "Condition.h"

class QuestObjectiveResolution
{
public:
    QuestObjectiveResolution(CYIString description);
    ~QuestObjectiveResolution();

    static QuestObjectiveResolution* FromJSON(const yi::rapidjson::Value& resolutionJSONObject);

    std::vector<Condition>*  GetConditions() const;
    bool IsFulfilled() const;

    void FullfillCondition(CYIString condition);

    CYIString ToString() const;

private:
    CYIString m_description;
    std::vector<Condition>* m_conditions;
};

#endif
