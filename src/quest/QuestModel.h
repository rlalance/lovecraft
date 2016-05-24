#ifndef QUEST_MODEL_H
#define QUEST_MODEL_H

#include "QuestObjectiveModel.h"

#include <utility\YiString.h>
#include <utility\YiRapidJSONUtility.h>
#include <datamodel\YiAbstractDataModel.h>

class QuestModel : CYIAbstractDataModel
{
public:
    QuestModel(CYIString name, CYIString description);
    ~QuestModel();

    void AddRowsToMatchIndex(YI_INT32 index);
    void AddObjective(QuestObjectiveModel* objective, YI_INT32 index);

    static QuestModel* FromJSON(const yi::rapidjson::Value& jsonObject);

    CYIString ToString();

private:
    void Initialize(CYIString name, CYIString description);

    CYIString m_name;
    CYIString m_description;
};
#endif
