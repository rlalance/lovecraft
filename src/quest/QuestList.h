#ifndef QUEST_CONTROLLER_H
#define QUEST_CONTROLLER_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <utility\YiRapidJSONUtility.h>

#include "quest/QuestModel.h"

class QuestList : CYIAbstractDataModel
{
public:
    QuestList();
    ~QuestList();

    static QuestList* FromJSON(CYIString path);
    static QuestList* FromJSON(yi::rapidjson::Document* document);

    CYIString Display();

private:
    void AddRowsToMatchIndex(YI_INT32 index);
    void AddQuest(QuestModel* quest, YI_INT32 index);    
};
#endif