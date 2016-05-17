#include "quest/QuestObjectiveModel.h"

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

QuestObjectiveModel::QuestObjectiveModel(CYIString name, YI_INT32 numberStates) : CYIAbstractDataModel(numberStates, 1)
{
	this->name = name;

	if (numberStates <=1)//No good. An objective must at least have 2 states, incomplete and resolved.
	{		
		InsertRows(0, 2);
	}
	currentState = 0;
}

QuestObjectiveModel::~QuestObjectiveModel()
{
	
}