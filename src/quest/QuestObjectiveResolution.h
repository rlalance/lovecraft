#ifndef QUEST_OBJECTIVE_RESOLUTION_H
#define QUEST_OBJECTIVE_RESOLUTION_H

#include <rapidjson/document.h>
#include <utility/YiString.h>
#include "Condition.h"
#include <smartptr/YiSharedPtr.h>

class QuestObjectiveResolution
{
public:
    QuestObjectiveResolution(CYIString description);
    ~QuestObjectiveResolution();
    static QuestObjectiveResolution* FromJSON(const yi::rapidjson::Value& resolutionJSONObject);

    bool IsFulfilled() const;
    void FullfillCondition(CYIString condition);
    bool HasConditions() const;

    CYIString ToString() const;
    CYIString GetDisplayText() const;

private:
    void AddCondition(Condition* condition);

    CYIString m_description;
    std::vector<CYISharedPtr<Condition>> m_conditions;
};

#endif
