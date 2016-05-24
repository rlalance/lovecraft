#ifndef QUEST_OBJECTIVE_RESOLUTION_H
#define QUEST_OBJECTIVE_RESOLUTION_H

#include <rapidjson/document.h>
#include <utility/YiString.h>

class QuestObjectiveResolution
{
public:
    QuestObjectiveResolution(CYIString description, CYIString condition);
    ~QuestObjectiveResolution();

    static QuestObjectiveResolution* FromJSON(const yi::rapidjson::Value& resolutionJSONObject);

    CYIString ToString();

private:
    CYIString m_description;
    CYIString m_condition;
    bool m_bConditionFullfilled = false;
};

#endif
