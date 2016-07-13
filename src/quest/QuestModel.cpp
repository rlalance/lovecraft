#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include "QuestModel.h"
#include "QuestObjectiveModel.h"

QuestModel::QuestModel(CYIString name, CYIString description) : CYIAbstractDataModel(1)
{
    Initialize(name, description);
}

void QuestModel::Initialize(CYIString name, CYIString description)
{
    m_name = name;
    m_description = description;
}

QuestModel::~QuestModel()
{
}

bool QuestModel::PreconditionsFulfilled() const
{
    bool fulfilled = true;
    for (CYISharedPtr<Condition> precondition : m_preconditions)
    {
        fulfilled = fulfilled && precondition->IsFulfilled();
    }

    return fulfilled;
}

void QuestModel::AddRowsToMatchIndex(YI_INT32 index)
{
    YI_INT32 n_row = GetRowCount();
    YI_INT32 missingRows = (index + 1) - n_row;

    if (missingRows > 0)
    {
        InsertRows(n_row, missingRows);
    }
}

void QuestModel::AddObjective(QuestObjectiveModel* objective, YI_INT32 index)
{
    AddRowsToMatchIndex(index);

    if (HasIndex(index, 0))
    {
        CYISharedPtr<QuestObjectiveModel> objectivePtr = CYISharedPtr<QuestObjectiveModel>(objective);
        SetItemData(GetIndex(index, 0), CYIAny(objectivePtr));
    }
}

void QuestModel::AddPrecondition(Condition* precondition)
{
    m_preconditions.push_back(CYISharedPtr<Condition>(precondition));
}

QuestModel* QuestModel::FromJSON(const yi::rapidjson::Value& questJSONObject)
{
    CYIParsingError parsingError;

    CYIString questName;
    CYIString questDescription;

    CYIRapidJSONUtility::GetStringField(&questJSONObject, "Name", questName, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestModel::FromJSON", parsingError.GetParsingErrorMessage());

    CYIRapidJSONUtility::GetStringField(&questJSONObject, "Description", questDescription, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestModel::FromJSON", parsingError.GetParsingErrorMessage());

    QuestModel* newQuest = new QuestModel(questName, questDescription);

    const yi::rapidjson::Value& preconditions = questJSONObject["Preconditions"];
    YI_ASSERT(preconditions.IsArray(), "QuestList::FromJSON", "Could not find preconditions array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < preconditions.Size(); ++i)
    {
        const yi::rapidjson::Value& precondition = preconditions[i];

        newQuest->AddPrecondition(new Condition(precondition.GetString()));
    }

    const yi::rapidjson::Value& objectives = questJSONObject["Objectives"];
    YI_ASSERT(objectives.IsArray(), "QuestModel::FromJSON", "Could not find objectives array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < objectives.Size(); ++i)
    {
        const yi::rapidjson::Value& objective = objectives[i];

        newQuest->AddObjective(QuestObjectiveModel::FromJSON(objective), i);
    }

    return newQuest;
}

void QuestModel::Trigger(CYIString condition)
{
    for (CYISharedPtr<Condition> p_condition : m_preconditions)
    {
        if (p_condition->GetCondition() == condition)
        {
            p_condition->Trigger();
        }

    }

    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveModel> objective = data.Get<CYISharedPtr<QuestObjectiveModel>>();
            
            objective.Get()->Trigger(condition);
        }
    }
}

CYIString QuestModel::GetDisplayText()
{
    CYIString questInfo;
    CYIString objectivesInfo;
    bool foundUnresolvedObjective = false;

    if (PreconditionsFulfilled())
    {        
        for (YI_INT32 i = 0; i < GetRowCount() && !foundUnresolvedObjective; ++i)
        {
            CYIAny data(GetItemData(GetIndex(i, 0)));

            if (!data.Empty())
            {
                CYISharedPtr<QuestObjectiveModel> objective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

                if (!objective->IsResolved())
                {
                    foundUnresolvedObjective = true;
                }

                objectivesInfo.Append(objective->GetDisplayText() + "\n");
            }
        }

        questInfo.Append(m_name);

        if (!foundUnresolvedObjective)
        {
            questInfo.Append(" (Complete)");
        }
        questInfo.Append(": " + m_description + "\n");
        questInfo.Append(objectivesInfo);
    }
    return questInfo;
}

CYIString QuestModel::ToString()
{
    CYIString questInfo;
    questInfo.Append("QuestName: " + m_name + "\n");
    questInfo.Append("QuestDescription: " + m_description + "\n");

    questInfo.Append("Preconditions: [");
    for (YI_UINT32 i = 0; i < m_preconditions.size(); ++i)
    {        
        questInfo.Append(std::to_string(i) + ": " + m_preconditions[i]->ToString());
        
        if(i < m_preconditions.size()-1)
        {
            questInfo.Append(", ");
        }
    }
    questInfo.Append("]\n");

    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveModel> objective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

            questInfo.Append(objective->ToString() + "\n");
        }
    }

    return questInfo;
}
