#include "QuestManager.h"

QuestManager::QuestManager(CYIString questFilePath)
{
    Initialize(questFilePath);
}

QuestManager::~QuestManager()
{
    delete m_pQuests;
}

CYIString QuestManager::AllQuestsToString() const
{
    return m_pQuests->ToString();
}

void QuestManager::Initialize(CYIString path)
{
    LoadQuestsFromFile(path);
}

void QuestManager::LoadQuestsFromFile(CYIString path)
{
    m_pQuests = QuestList::FromJSON(path);
}