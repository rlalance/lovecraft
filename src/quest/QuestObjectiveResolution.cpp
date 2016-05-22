#include "QuestObjectiveResolution.h"
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiError.h>

QuestObjectiveResolution::QuestObjectiveResolution(CYIString description, CYIString condition)
{
    m_description = description;
    m_condition = condition;
}

QuestObjectiveResolution::~QuestObjectiveResolution()
{
    
}

QuestObjectiveResolution* QuestObjectiveResolution::FromJSON(const yi::rapidjson::Value& resolutionJSONObject)
{
    CYIParsingError parsingError;

    CYIString description;
    CYIString condition;

    CYIRapidJSONUtility::GetStringField(&resolutionJSONObject, "Name", description, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveResolution::FromJSON", parsingError.GetParsingErrorMessage());

    CYIRapidJSONUtility::GetStringField(&resolutionJSONObject, "Name", condition, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveResolution::FromJSON", parsingError.GetParsingErrorMessage());

    return new QuestObjectiveResolution(description, condition);
}