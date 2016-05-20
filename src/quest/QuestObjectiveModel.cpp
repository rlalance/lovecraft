#include "quest/QuestObjectiveModel.h"

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

QuestObjectiveModel::QuestObjectiveModel(CYIString name) : CYIAbstractDataModel(1)
{
    Initialize(name);
}

void QuestObjectiveModel::Initialize(CYIString name)
{
    m_name = name;
    m_ncurrentState = -1;
}

QuestObjectiveModel::~QuestObjectiveModel()
{

}

void QuestObjectiveModel::SetUnresolvedText(CYIString text)
{
    if (GetRowCount() == 0)
    {
        InsertRow(0);
    }

    SetResolutionText(0, text);
    m_ncurrentState = 0;
}

void QuestObjectiveModel::AddResolutionText(CYIString text)
{
    YI_INT32 count = GetRowCount();
    YI_INT32 columns = GetColumnCount();
    InsertRow(count);
    count = GetRowCount();
    CYIDataModelIndex index = GetIndex(count - 1, 0);
    CYIAny resolutionText = CYIAny(text);
    
    if (index.IsValid()) // Comes out invalid despite being within the bounds of the datamodel
    {
        SetItemData(index, resolutionText);
    }
}

CYIString QuestObjectiveModel::ToString()
{
    CYIString objectiveInfo;
    objectiveInfo.Append(m_name);
    objectiveInfo.Append(CYIString("\n["));

    YI_INT32 count = GetRowCount();

    for (YI_INT32 i = 0; i < count; i++)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            objectiveInfo.Append(std::to_string(i) + ":");
            CYIString resolution = data.Get<CYIString>();
            objectiveInfo.Append(resolution);
            
            if (i < GetRowCount()-1)
            {
                objectiveInfo.Append(" ");
            }
        }
    }

    objectiveInfo.Append(CYIString("]"));
    return objectiveInfo;
}

void QuestObjectiveModel::SetResolutionText(YI_UINT32 index, CYIString text)
{
    CYIDataModelIndex cellIndex = GetIndex(index, 0);
    CYIAny resolutionText = CYIAny(text);

    SetItemData(cellIndex, resolutionText);
}