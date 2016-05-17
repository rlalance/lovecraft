#ifndef QUEST
#define QUEST

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

class QuestModel : CYIAbstractDataModel
{
public:
	QuestModel(CYIString name, CYIString description);
//	QuestModel(CYIString name, CYIString description, YI_INT32 nObjectives);
	~QuestModel();

	void AddObjective(CYIString name, const std::vector<CYIString> resolutions);

	CYIString ToString() const;
	
	//Test Method
	void PopulateAndRead();
private:
	CYIString name;
	CYIString description;
};

#endif