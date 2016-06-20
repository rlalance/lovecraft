#ifndef QUEST_MODEL_H
#define QUEST_MODEL_H

#include "QuestObjectiveModel.h"

#include <utility\YiString.h>
#include <utility\YiRapidJSONUtility.h>
#include <datamodel\YiAbstractDataModel.h>
#include "Condition.h"

class QuestModel : CYIAbstractDataModel
{
public:
    QuestModel(CYIString name, CYIString description);
    ~QuestModel();

    bool PreconditionsFulfilled() const;

    void AddRowsToMatchIndex(YI_INT32 index);
    void AddObjective(QuestObjectiveModel* objective, YI_INT32 index);

    void AddPrecondition(Condition* precondition);
    static QuestModel* FromJSON(const yi::rapidjson::Value& jsonObject);

    void Trigger(CYIString condition);

    CYIString GetDisplayText();
    CYIString ToString();

private:
    void Initialize(CYIString name, CYIString description);
    std::vector<CYISharedPtr<Condition>> m_preconditions;

    CYIString m_name;
    CYIString m_description;
};
#endif
