#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include "QuestList.h"

class QuestManager
{
public:
    QuestManager(CYIString questFilePath);
    ~QuestManager();

    void Trigger(CYIString condition);

    CYIString AllQuestsToString() const;
    CYIString GetAllQuestsDisplayText() const;

private:
    QuestList *m_pQuests;

    void Initialize(CYIString path);
    void LoadQuestsFromFile(CYIString path);
};

#endif
