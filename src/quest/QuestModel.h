#ifndef QUEST_MODEL_H
#define QUEST_MODEL_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <utility\YiRapidJSONUtility.h>

#include "QuestObjectiveModel.h"

class QuestModel : CYIAbstractDataModel
{
public:
    QuestModel(CYIString name, CYIString description);
    ~QuestModel();

    void AddRowsToMatchIndex(YI_INT32 index);
    void AddObjective(QuestObjectiveModel* objective, YI_INT32 index);

    static QuestModel* FromJSON(const yi::rapidjson::Value& jsonObject);

    CYIString Display();

private:
    void Initialize(CYIString name, CYIString description);

    CYIString m_name;
    CYIString m_description;
};

#endif