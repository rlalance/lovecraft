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

    bool IsAvailable() const;
    bool IsAccepted() const;
    bool IsCompleted() const;

    void Accept();

    void AddRowsToMatchIndex(YI_INT32 index);
    void AddObjective(QuestObjectiveModel *objective, YI_INT32 nIndex);
    void AddPrecondition(Condition *precondition);

    static QuestModel *FromJSON(const yi::rapidjson::Value &jsonObject);

    void ActivatePreCondition(CYIString condition);
    void ActivateCondition(CYIString condition);

    CYIString GetDisplayText() const;
    CYIString ToString() const;

private:
    void Initialize(CYIString name, CYIString description);
    std::vector<CYISharedPtr<Condition>> m_preconditions;

    CYIString m_name;
    CYIString m_description;
    bool m_bAccepted;
};
#endif
