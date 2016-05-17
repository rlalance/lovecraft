#ifndef QUEST_PARSER
#define QUEST_PARSER

#include "QuestModel.h"

#include <utility\YiRapidJSONUtility.h>

class QuestParser
{
public:
	static QuestModel* ParseQuest(const CYIString &filepath)
	{
		//Load the file --------------------
		yi::rapidjson::Document* document;
		CYIParsingError parsingError;

		document = CYIRapidJSONUtility::CreateDocumentFromFile(filepath, parsingError);
		YI_ASSERT(!parsingError.HasError(), "QuestParser", parsingError.GetParsingErrorMessage());

		const yi::rapidjson::Value& quest = (*document)["Quest"];

		//Parse quest info --------------------
		CYIString questName;
		CYIString questDescription;

		CYIRapidJSONUtility::GetStringField(&quest, "Name", questName, parsingError);
		YI_ASSERT(!parsingError.HasError(), "QuestParser", parsingError.GetParsingErrorMessage());

		CYIRapidJSONUtility::GetStringField(&quest, "Description", questDescription, parsingError);
		YI_ASSERT(!parsingError.HasError(), "QuestParser", parsingError.GetParsingErrorMessage());

		QuestModel* newQuest = new QuestModel(questName, questDescription);

		//Parse Objectives --------------------
		const yi::rapidjson::Value& objectives = quest["Objectives"];

		for (yi::rapidjson::SizeType i = 0; i<objectives.Size(); ++i)
		{			
			const yi::rapidjson::Value& objective = objectives[i];

			CYIString name;
			CYIRapidJSONUtility::GetStringField(&objective, "Name", name, parsingError);
			YI_ASSERT(!parsingError.HasError(), "QuestParser", parsingError.GetParsingErrorMessage());

			const yi::rapidjson::Value& resolutions = objective["Resolution"];

			std::vector<CYIString> questResolutionList;
			for (yi::rapidjson::SizeType r = 0; r<resolutions.Size(); ++r)
			{
				questResolutionList.push_back(resolutions[r].GetString());
			}

			newQuest->AddObjective(name, questResolutionList);
		}

		return newQuest;
	}
};
#endif
