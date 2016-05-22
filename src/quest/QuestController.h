#ifndef QUEST_CONTROLLER_H
#define QUEST_CONTROLLER_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <utility\YiRapidJSONUtility.h>

#include "quest/QuestModel.h"

class QuestController : CYIAbstractDataModel
{
public:
    QuestController();
    ~QuestController();

    void FromJSON(CYIString path);
    void FromJSON(yi::rapidjson::Document* document);

//    yi::rapidjson::Document* ToJSON();

private:
    void AddQuest(QuestModel* quest);

};
#endif