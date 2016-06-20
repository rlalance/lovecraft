#include "QuestObjectiveResolution.h"
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiError.h>

QuestObjectiveResolution::QuestObjectiveResolution(CYIString description)
{
    m_description = description;
    m_conditions = new std::vector<Condition>();
}

QuestObjectiveResolution::~QuestObjectiveResolution()
{
    delete m_conditions;
}

QuestObjectiveResolution* QuestObjectiveResolution::FromJSON(const yi::rapidjson::Value& resolutionJSONObject)
{
    CYIParsingError parsingError;

    CYIString description;
    CYIString condition;

    CYIRapidJSONUtility::GetStringField(&resolutionJSONObject, "Description", description, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveResolution::FromJSON", parsingError.GetParsingErrorMessage());

    CYIRapidJSONUtility::GetStringField(&resolutionJSONObject, "Resolution_Condition", condition, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveResolution::FromJSON", parsingError.GetParsingErrorMessage());

    return new QuestObjectiveResolution(description);
}

std::vector<Condition>* QuestObjectiveResolution::GetConditions() const
{
    return m_conditions;
}

bool QuestObjectiveResolution::IsFulfilled() const
{
    bool fulfilled = false;

    for (int i = 0; i < m_conditions->size(); ++i)
    {
        Condition condition = m_conditions;
        fulfilled = fulfilled && condition.IsFulfilled();
    }

}

void QuestObjectiveResolution::FullfillCondition(CYIString condition)
{
    if (m_condition == condition)
    {
        m_bConditionFullfilled = true;
    }
}

CYIString QuestObjectiveResolution::ToString() const
{
    CYIString resolutionInfo;

    resolutionInfo.Append(m_description + " (" + m_condition + ")");

    return resolutionInfo;
}

