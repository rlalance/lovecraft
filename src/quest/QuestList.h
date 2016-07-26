#ifndef QUEST_LIST_H
#define QUEST_LIST_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <utility\YiRapidJSONUtility.h>

#include "quest/QuestModel.h"

class QuestList : CYIAbstractDataModel
{
public:
    QuestList();
    ~QuestList();

    static QuestList *FromJSON(CYIString path);
    static QuestList *FromJSON(yi::rapidjson::Document *document);

    void ActivateCondition(CYIString condition);

    CYIString GetDisplayText();
    CYIString ToString();

private:
    void AddRowsToMatchIndex(YI_INT32 nIndex);
    void AddQuest(QuestModel *quest, YI_INT32 nIndex);  
};
#endif
