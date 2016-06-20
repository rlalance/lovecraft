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
}

QuestObjectiveResolution* QuestObjectiveResolution::FromJSON(const yi::rapidjson::Value& resolutionJSONObject)
{
    CYIParsingError parsingError;

    CYIString description;

    CYIRapidJSONUtility::GetStringField(&resolutionJSONObject, "Description", description, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveResolution::FromJSON", parsingError.GetParsingErrorMessage());

    QuestObjectiveResolution* quest_objective_resolution = new QuestObjectiveResolution(description);

    const yi::rapidjson::Value& conditions = resolutionJSONObject["Conditions"];
    YI_ASSERT(conditions.IsArray(), "QuestList::FromJSON", "Could not find conditions array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < conditions.Size(); ++i)
    {
        const yi::rapidjson::Value& condition = conditions[i];

        quest_objective_resolution->AddCondition(new Condition(condition.GetString()));
    }

    return quest_objective_resolution;
}

bool QuestObjectiveResolution::IsFulfilled() const
{
    bool fulfilled = false;
    for (CYISharedPtr<Condition> condition : m_conditions)
    {
        fulfilled = fulfilled && condition->IsFulfilled();
    }

    return fulfilled;
}

void QuestObjectiveResolution::FullfillCondition(CYIString condition)
{
    for (CYISharedPtr<Condition> p_condition : m_conditions)
    {
        if (p_condition->GetCondition() == condition)
        {
            p_condition->Trigger();
        }
    }
}

CYIString QuestObjectiveResolution::ToString() const
{
    CYIString resolutionInfo;

    resolutionInfo.Append(m_description + "\n");

    resolutionInfo.Append("Preconditions: [");
    for (YI_UINT32 i = 0; i < m_conditions.size(); ++i)
    {
        resolutionInfo.Append(std::to_string(i) + ": " + m_conditions[i]->ToString());

        if (i < m_conditions.size() - 1)
        {
            resolutionInfo.Append(",");
        }
    }
    resolutionInfo.Append("]\n");

    return resolutionInfo;
}

void QuestObjectiveResolution::AddCondition(Condition* condition)
{
    m_conditions.push_back(CYISharedPtr<Condition>(condition));
}
