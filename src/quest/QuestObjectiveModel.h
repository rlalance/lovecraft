#ifndef QUEST_OBJECTIVE_MODEL_H
#define QUEST_OBJECTIVE_MODEL_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include <rapidjson/document.h>
#include "QuestObjectiveResolution.h"

/*
This Class is a model which contains all the possible resolutions of an objective, as well as a name for the objective itself and a pointer to the current state.
*/
class QuestObjectiveModel : CYIAbstractDataModel
{
public:
    QuestObjectiveModel(CYIString name);
    ~QuestObjectiveModel();

    void SetUnresolvedText(CYIString text);
    void AddResolutionText(CYIString text);

    void AddResolution(QuestObjectiveResolution* resolution, YI_INT32 index);

    CYIString ToString();
    static QuestObjectiveModel* FromJSON(const yi::rapidjson::Value& objectiveJSONObject);
//    yi::rapidjson::Document* ToJSON();

private:
    void Initialize(CYIString name);

    void SetResolutionText(YI_UINT32 index, CYIString text);

    CYIString m_name;
};

#endif
