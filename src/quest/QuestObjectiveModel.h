#ifndef QUEST_OBJECTIVE_MODEL
#define QUEST_OBJECTIVE_MODEL

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

/*
This Class is a model which contains all the possible resolutions of an objective, as well as a name for the objective itself and a pointer to the current state.
*/
class QuestObjectiveModel : CYIAbstractDataModel
{
public:
	QuestObjectiveModel(CYIString name, YI_INT32 numberStates);
	~QuestObjectiveModel();
private: 
	CYIString name;

	//I wanted to use the specialized index class, but it has no public constructors, therefore the compiler would not buck at this class's constructor.
	YI_INT32 currentState;
};

#endif
