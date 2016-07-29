#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include "QuestList.h"
#include <signal/YiSignalHandler.h>

class QuestManager : CYISignalHandler
{
public:
    QuestManager(CYIString questFilePath);
    ~QuestManager();

    void ActivateCondition(CYIString condition);

    CYIString AllQuestsToString() const;
    CYIString GetAllQuestsDisplayText() const;

    //Signals
    static CYISignal<QuestModel *> QuestAvailableSig;
    static CYISignal<QuestModel *> QuestAcceptedSig;
    static CYISignal<QuestObjectiveModel *> ObjectiveCompletedSig;
    static CYISignal<QuestModel *> QuestCompletedSig;

private:
    QuestList *m_pQuests;

    void Initialize(CYIString path);
    void LoadQuestsFromJSON(CYIString path);
};

#endif
