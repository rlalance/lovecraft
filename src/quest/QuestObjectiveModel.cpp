#include "QuestObjectiveModel.h"

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <utility/YiRapidJSONUtility.h>
#include "QuestObjectiveResolution.h"

QuestObjectiveModel::QuestObjectiveModel(CYIString name) : CYIAbstractDataModel(1)
{
    Initialize(name);
}

void QuestObjectiveModel::Initialize(CYIString name)
{
    m_name = name;
}

QuestObjectiveModel::~QuestObjectiveModel()
{

}

void QuestObjectiveModel::AddRowsToMatchIndex(YI_INT32 index)
{
    YI_INT32 n_row = GetRowCount();
    YI_INT32 missingRows = (index + 1) - n_row;

    if (missingRows > 0)
    {
        InsertRows(n_row, missingRows);
    }
}

void QuestObjectiveModel::AddResolution(QuestObjectiveResolution *pObjectiveResolution, YI_INT32 nIndex)
{
    AddRowsToMatchIndex(nIndex);

    if (HasIndex(nIndex, 0))
    {
        SetItemData(GetIndex(nIndex, 0), CYIAny(CYISharedPtr<QuestObjectiveResolution>(pObjectiveResolution)));
    }
}

void QuestObjectiveModel::ActivateCondition(CYIString condition)
{
    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveResolution> pObjectiveResolution = data.Get<CYISharedPtr<QuestObjectiveResolution>>();

            pObjectiveResolution->FullfillCondition(condition);
        }
    }
}

bool QuestObjectiveModel::IsResolved() const
{
    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveResolution> pObjectiveResolution = data.Get<CYISharedPtr<QuestObjectiveResolution>>();

            if (pObjectiveResolution->HasConditions())
            {
                if (pObjectiveResolution->IsFulfilled())
                {
                    return true;
                }
            }
        }
    }

    return false;
}

CYIString QuestObjectiveModel::GetDisplayText()
{
    CYIString displayText;
    YI_INT32 resolutionIndex = 0;

    displayText.Append("\t" + m_name + ": ");

    for (YI_INT32 i = 0; i < GetRowCount() && resolutionIndex == 0; ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveResolution> pObjectiveResolution = data.Get<CYISharedPtr<QuestObjectiveResolution>>();

            if (pObjectiveResolution->HasConditions() && pObjectiveResolution->IsFulfilled())
            {
                resolutionIndex = i;
            }            
        }
    }

    CYIAny data(GetItemData(GetIndex(resolutionIndex, 0)));
    CYISharedPtr<QuestObjectiveResolution> resolution = data.Get<CYISharedPtr<QuestObjectiveResolution>>();

    displayText.Append(resolution->GetDisplayText() + "\n");

    return displayText;
}

QuestObjectiveModel *QuestObjectiveModel::FromJSON(const yi::rapidjson::Value& objectiveJSONObject)
{
    CYIParsingError parsingError;

    CYIString name;

    CYIRapidJSONUtility::GetStringField(&objectiveJSONObject, "Name", name, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestObjectiveModel::FromJSON", parsingError.GetParsingErrorMessage());

    QuestObjectiveModel *pNewObjective = new QuestObjectiveModel(name);

    const yi::rapidjson::Value &resolutions = objectiveJSONObject["Resolutions"];
    YI_ASSERT(resolutions.IsArray(), "QuestModel::FromJSON", "Could not find resolutions array in JSON file.");

    std::vector<CYIString> questResolutionList;

    for (yi::rapidjson::SizeType r = 0; r < resolutions.Size(); ++r)
    {
        const yi::rapidjson::Value &resolution = resolutions[r];

        pNewObjective->AddResolution(QuestObjectiveResolution::FromJSON(resolution), r);
    }

    return pNewObjective;
}

CYIString QuestObjectiveModel::ToString()
{
    CYIString objectiveInfo;
    objectiveInfo.Append("ObjectiveName: " + m_name + "\n");

    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestObjectiveResolution> pObjectiveResolution = data.Get<CYISharedPtr<QuestObjectiveResolution>>();

            objectiveInfo.Append(std::to_string(i) + ": " + pObjectiveResolution->ToString() + "\n");
        }
    }

    return objectiveInfo;
}
