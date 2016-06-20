#include "QuestManager.h"

QuestManager::QuestManager(CYIString questFilePath)
{
    Initialize(questFilePath);
}

QuestManager::~QuestManager()
{
    delete m_pQuests;
}

void QuestManager::Trigger(CYIString condition)
{
    m_pQuests->Trigger(condition);
}

CYIString QuestManager::AllQuestsToString() const
{
    return m_pQuests->ToString();
}

CYIString QuestManager::GetAllQuestsDisplayText() const
{
    return m_pQuests->GetDisplayText();
}

void QuestManager::Initialize(CYIString path)
{
    LoadQuestsFromFile(path);
}

void QuestManager::LoadQuestsFromFile(CYIString path)
{
    m_pQuests = QuestList::FromJSON(path);
}