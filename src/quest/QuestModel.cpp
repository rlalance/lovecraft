#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include "QuestModel.h"
#include "QuestObjectiveModel.h"
#include "QuestManager.h"

QuestModel::QuestModel(CYIString name, CYIString description) : 
    CYIAbstractDataModel(1), 
    m_name(name), 
    m_description(description)
{
    m_bAccepted = false;
}

void QuestModel::Initialize(CYIString name, CYIString description)
{
    m_name = name;
    m_description = description;
    m_bAccepted = false;
}

QuestModel::~QuestModel()
{
}

bool QuestModel::IsAvailable() const
{
    bool bAvailable = true;

    for (CYISharedPtr<Condition> precondition : m_preconditions)
    {
        bAvailable = bAvailable && precondition->IsFulfilled();
    }

    return bAvailable;
}

bool QuestModel::IsAccepted() const
{
    return m_bAccepted;
}

bool QuestModel::IsCompleted() const
{
    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveModel> pObjective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

            if (!pObjective->IsResolved())
            {
                return false;
            }
        }
    }

    return true;
}

void QuestModel::Accept()
{
    if (IsAvailable() && !m_bAccepted)
    {
        m_bAccepted = true;

        QuestManager::QuestAcceptedSig(this);
    }
}

void QuestModel::AddRowsToMatchIndex(YI_INT32 nIndex)
{
    YI_INT32 nRowCount = GetRowCount();
    YI_INT32 nMissingRows = (nIndex + 1) - nRowCount;

    if (nMissingRows > 0)
    {
        InsertRows(nRowCount, nMissingRows);
    }
}

void QuestModel::AddObjective(QuestObjectiveModel *objective, YI_INT32 nIndex)
{
    AddRowsToMatchIndex(nIndex);

    if (HasIndex(nIndex, 0))
    {
        CYISharedPtr<QuestObjectiveModel> pObjective = CYISharedPtr<QuestObjectiveModel>(objective);
        SetItemData(GetIndex(nIndex, 0), CYIAny(pObjective));
    }
}

void QuestModel::AddPrecondition(Condition *pPreCondition)
{
    m_preconditions.push_back(CYISharedPtr<Condition>(pPreCondition));
}

QuestModel* QuestModel::FromJSON(const yi::rapidjson::Value &questJSONObject)
{
    CYIParsingError parsingError;

    CYIString questName;
    CYIString questDescription;

    CYIRapidJSONUtility::GetStringField(&questJSONObject, "Name", questName, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestModel::FromJSON", parsingError.GetParsingErrorMessage());

    CYIRapidJSONUtility::GetStringField(&questJSONObject, "Description", questDescription, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestModel::FromJSON", parsingError.GetParsingErrorMessage());

    QuestModel* pNewQuest = new QuestModel(questName, questDescription);

    const yi::rapidjson::Value& preconditions = questJSONObject["Preconditions"];
    YI_ASSERT(preconditions.IsArray(), "QuestList::FromJSON", "Could not find preconditions array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < preconditions.Size(); ++i)
    {
        const yi::rapidjson::Value& precondition = preconditions[i];

        pNewQuest->AddPrecondition(new Condition(precondition.GetString()));
    }

    const yi::rapidjson::Value& objectives = questJSONObject["Objectives"];
    YI_ASSERT(objectives.IsArray(), "QuestModel::FromJSON", "Could not find objectives array in JSON file.");

    for (yi::rapidjson::SizeType i = 0; i < objectives.Size(); ++i)
    {
        const yi::rapidjson::Value& objective = objectives[i];

        pNewQuest->AddObjective(QuestObjectiveModel::FromJSON(objective), i);
    }

    return pNewQuest;
}

void QuestModel::ActivatePreCondition(CYIString condition)
{
    if (!IsAvailable())
    {
        for (CYISharedPtr<Condition> pPreCondition : m_preconditions)
        {
            if (pPreCondition->GetCondition() == condition)
            {
                pPreCondition->Activate();
            }
        }

        if (IsAvailable())
        {
            QuestManager::QuestAvailableSig(this);
        }
    }
}

void QuestModel::ActivateCondition(CYIString condition)
{
    if (!IsCompleted())
    {
        for (YI_INT32 i = 0; i < GetRowCount(); ++i)
        {
            CYIAny data(GetItemData(GetIndex(i, 0)));

            if (!data.Empty())
            {
                CYISharedPtr<QuestObjectiveModel> pObjective = data.Get<CYISharedPtr<QuestObjectiveModel>>();
            
                pObjective->ActivateCondition(condition);
            }
        }        

        if (IsCompleted())
        {
            QuestManager::QuestCompletedSig(this);
        }
    }
}

CYIString QuestModel::GetDisplayText() const
{
    CYIString questInfo;
    CYIString objectivesInfo;
    bool foundUnresolvedObjective = false;

    if (IsAvailable())
    {        
        for (YI_INT32 i = 0; i < GetRowCount() && !foundUnresolvedObjective; ++i)
        {
            CYIAny data(GetItemData(GetIndex(i, 0)));

            if (!data.Empty())
            {
                CYISharedPtr<QuestObjectiveModel> pObjective = data.Get<CYISharedPtr<QuestObjectiveModel>>();

                if (!pObjective->IsResolved())
                {
                    foundUnresolvedObjective = true;
                }

                objectivesInfo.Append(pObjective->GetDisplayText() + "\n");
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

CYIString QuestModel::ToString() const
{
    CYIString questInfo;
    questInfo.Append("QuestName: " + m_name + "\n");
    questInfo.Append("QuestDescription: " + m_description + "\n");

    questInfo.Append("Preconditions: [");

    for (YI_UINT32 i = 0; i < m_preconditions.size(); ++i)
    {        
        questInfo.Append(std::to_string(i) + ": " + m_preconditions[i]->ToString());
        
        if (i < m_preconditions.size()-1)
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
