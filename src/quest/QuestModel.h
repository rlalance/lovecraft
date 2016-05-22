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

    void AddObjective(CYIString name, const std::vector<CYIString> resolutions);
    void AddObjective(QuestObjectiveModel* objective);

    CYIString ToString() const;

    static QuestModel* FromJSON(const yi::rapidjson::Value& jsonObject);

    yi::rapidjson::Document* ToJSON();

private:
    void Initialize(CYIString name, CYIString description);

    CYIString m_name;
    CYIString m_description;
};

#endif