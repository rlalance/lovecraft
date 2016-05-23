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

    const yi::rapidjson::Value& objectives = questJSONObject["Objectives"];
    YI_ASSERT(objectives.IsArray(), "QuestModel::FromJSON", "Could not find objectives array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < objectives.Size(); ++i)
    {
        const yi::rapidjson::Value& objective = objectives[i];

        newQuest->AddObjective(QuestObjectiveModel::FromJSON(objective), i);
    }

    return newQuest;
}

yi::rapidjson::Document* ToJSON();

CYIString QuestModel::Display()
{
    CYIString questInfo;
    questInfo.Append("QuestName: " + m_name + "\n");
    questInfo.Append("QuestDescription: " + m_description + "\n");

    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveModel> objective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

            questInfo.Append(objective->Display() + "\n");
        }
    }

    return questInfo;
}