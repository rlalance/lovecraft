#ifndef QUEST_MODEL_H
#define QUEST_MODEL_H

#include <utility\YiString.h>
#include <datamodel\YiAbstractDataModel.h>

class QuestModel : CYIAbstractDataModel
{
public:
    QuestModel(CYIString name, CYIString description);
    ~QuestModel();

    void AddObjective(CYIString name, const std::vector<CYIString> resolutions);

    CYIString ToString() const;

    //Test Method
    void PopulateAndRead();

private:
    void Initialize(CYIString name, CYIString description);

    CYIString m_name;
    CYIString m_description;
};

#endif