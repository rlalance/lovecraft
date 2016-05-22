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

void QuestModel::AddObjective(CYIString name, const std::vector<CYIString> resolutions)
{
    InsertRow(GetRowCount());

    CYIDataModelIndex index = GetIndex(GetRowCount()-1, 0);

    if (index.IsValid())
    {
        CYISharedPtr<QuestObjectiveModel> pQuestObjectiveModel(new QuestObjectiveModel(name));

        for (YI_UINT32 i = 0; i < resolutions.size(); ++i)
        {
            if (i == 0)
            {
                pQuestObjectiveModel->SetUnresolvedText(resolutions[i]);
            }
            else
            {
                pQuestObjectiveModel->AddResolutionText(resolutions[i]);
            }
        }

        CYIAny objective(pQuestObjectiveModel);

        SetItemData(index, objective);
    }
}

void QuestModel::AddObjective(QuestObjectiveModel* objective)
{
    
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

    for (yi::rapidjson::SizeType i = 0; i < objectives.Size(); ++i)
    {
        const yi::rapidjson::Value& objective = objectives[i];

        newQuest->AddObjective(QuestObjectiveModel::FromJSON(objective));
    }

    return newQuest;
}

yi::rapidjson::Document* ToJSON();

CYIString QuestModel::ToString() const
{
    CYIString questInfo;
    questInfo.Append(m_name);
    questInfo.Append(CYIString("\n"));
    questInfo.Append(m_description);

    //    YI_INT32 count = GetRowCount();

    for (YI_INT32 i = 1; i < GetRowCount(); i++) //Skipping the first one because it is empty. Design flaw.
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveModel> objective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

            questInfo.Append("\n");
            questInfo.Append(objective->ToString());
        }
    }

    return questInfo;
}