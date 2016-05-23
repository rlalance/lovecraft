#include "QuestList.h"

QuestList::QuestList() : CYIAbstractDataModel(1)
{
    
}

QuestList::~QuestList()
{
    
}

QuestList* QuestList::FromJSON(CYIString path)
{
    yi::rapidjson::Document* document;
    CYIParsingError parsingError;

    document = CYIRapidJSONUtility::CreateDocumentFromFile(path, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestList::FromJSON", parsingError.GetParsingErrorMessage());

    return FromJSON(document);
}

QuestList* QuestList::FromJSON(yi::rapidjson::Document* document)
{
    CYIParsingError parsingError;

    yi::rapidjson::Value& questArray = (*document)["Quests"];
    YI_ASSERT(questArray.IsArray(), "QuestList::FromJSON", "Could not find quests array in JSON file.");

    QuestList* questList = new QuestList();

    for (yi::rapidjson::SizeType i = 0; i < questArray.Size(); ++i) 
    {
        const yi::rapidjson::Value& questObject = questArray[i];

        questList->AddQuest(QuestModel::FromJSON(questObject), i);
    }

    return questList;
}

CYIString QuestList::Display()
{
    CYIString questListInfo;

    for (YI_INT32 i = 0; i < GetRowCount(); ++i)
    {
        CYIAny data(GetItemData(GetIndex(i, 0)));

        if (!data.Empty())
        {
            CYISharedPtr<QuestModel> quest = data.Get<CYISharedPtr<QuestModel>>();
            
            questListInfo.Append("\n");

            questListInfo.Append(quest->Display());
        }
    }

    return questListInfo;
}

void QuestList::AddRowsToMatchIndex(YI_INT32 index)
{
    YI_INT32 n_row = GetRowCount();
    YI_INT32 missingRows = (index + 1) - n_row;

    if (missingRows > 0)
    {
        InsertRows(n_row, missingRows);
    }
}

void QuestList::AddQuest(QuestModel* quest, YI_INT32 index)
{
    AddRowsToMatchIndex(index);

    if (HasIndex(index, 0))
    {
        CYISharedPtr<QuestModel> questPtr = CYISharedPtr<QuestModel>(quest);
        SetItemData(GetIndex(index, 0), CYIAny(questPtr));
    }
}
