#include "QuestController.h"

QuestController::QuestController() : CYIAbstractDataModel(1)
{
    
}

QuestController::~QuestController()
{
    
}

void QuestController::FromJSON(CYIString path)
{
    yi::rapidjson::Document* document;
    CYIParsingError parsingError;

    document = CYIRapidJSONUtility::CreateDocumentFromFile(path, parsingError);
    YI_ASSERT(!parsingError.HasError(), "QuestParser", parsingError.GetParsingErrorMessage());

    FromJSON(document);
}

void QuestController::FromJSON(yi::rapidjson::Document* document)
{
    CYIParsingError parsingError;

    yi::rapidjson::Value& questArray = (*document)["Quests"];

    for (yi::rapidjson::SizeType i = 0; i < questArray.Size(); ++i) 
    {
        const yi::rapidjson::Value& questObject = questArray[i];

        AddQuest(QuestModel::FromJSON(questObject));
    }
}

//yi::rapidjson::Document* QuestController::ToJSON()
//{
//    
//}

void QuestController::AddQuest(QuestModel* quest)
{
    
}