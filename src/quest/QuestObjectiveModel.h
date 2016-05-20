#ifndef QUEST_OBJECTIVE_MODEL_H
#define QUEST_OBJECTIVE_MODEL_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

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

    CYIString ToString();
private:
    void Initialize(CYIString name);

    void SetResolutionText(YI_UINT32 index, CYIString text);

    CYIString m_name;
    YI_INT32 m_ncurrentState; //I wanted to use the specialized index class, but it has no public constructors.
};

#endif
