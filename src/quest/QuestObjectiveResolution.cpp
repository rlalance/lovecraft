#include "QuestObjectiveResolution.h"
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiError.h>

QuestObjectiveResolution::QuestObjectiveResolution(CYIString description, CYIString condition)
{
    m_description = description;
    m_condition = condition;
    m_bConditionFullfilled = false;
}

QuestObjectiveResolution::~QuestObjectiveResolution()
{
    
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

    return new QuestObjectiveResolution(description, condition);
}

CYIString QuestObjectiveResolution::Display()
{
    CYIString resolutionInfo;

    resolutionInfo.Append(m_description + " (" + m_condition + ")");

    return resolutionInfo;
}
