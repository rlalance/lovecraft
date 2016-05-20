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

//Test Method
void QuestModel::PopulateAndRead()
{
    //Populate
    YI_INT32 nItems = 10;
    CYIAbstractDataModel* model2 = new CYIAbstractDataModel(nItems, 1); //Now I know that this can be instantiated, even though I have yet to see an implementation of the class.

    CYIAbstractDataModel model(nItems, 1); // nItems rows, 1 column
    for (YI_INT32 i = 0; i < nItems; i++)
    {
        model2->SetItemData(model2->GetIndex(i, 0), CYIAny(CYIString::FromValue<YI_INT32>(i)));
    }

    //Reading a List of Strings :
    for (YI_INT32 i = 0; i < nItems; i++)
    {
        CYIAny listData = model2->GetItemData(model2->GetIndex(i, 0));
        YI_LOGD("DATAMODEL", "Contains %s at index %d", listData.Get<CYIString>().GetData(), i);
    }

    free(model2);
}