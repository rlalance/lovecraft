#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>
#include "quest/QuestModel.h"

QuestModel::QuestModel(CYIString name, CYIString description) : CYIAbstractDataModel(0, 1)
{
	this->name = name;
	this->description = description;
}
//
//QuestModel::QuestModel(CYIString name, CYIString description, YI_INT32 nObjectives) : CYIAbstractDataModel(nObjectives, 1)
//{
//	this->name = name;
//	this->description = description;
//}

QuestModel::~QuestModel()
{	
}

CYIString QuestModel::ToString() const
{
	//Quest Info
	CYIString result;
	result.Append(name);
	result.Append(CYIString("\n"));
	result.Append(description);

	return result;
}

void QuestModel::AddObjective(CYIString name, const std::vector<CYIString> resolutions)
{
	InsertRow(GetRowCount() - 1);
	
	
}


//Test Method
void QuestModel::PopulateAndRead()
{
	//Populate
	YI_INT32 nItems = 10;
	CYIAbstractDataModel* model2 = new CYIAbstractDataModel(nItems, 1); //Now I know that this can be instantiated, even though I have yet to see an implementation of the class.

	CYIAbstractDataModel model(nItems, 1); // nItems rows, 1 column
	for (YI_INT32 i = 0; i < nItems; i++)
	{
		model2->SetItemData(model2->GetIndex(i, 0), CYIAny(CYIString::FromValue<YI_INT32>(i)));
	}
				
	//Reading a List of Strings :
	for (YI_INT32 i = 0; i < nItems; i++)
	{
		CYIAny listData = model2->GetItemData(model2->GetIndex(i, 0));
		YI_LOGD("DATAMODEL", "Contains %s at index %d", listData.Get<CYIString>().GetData(), i);
	}

	free(model2);
}