#include "QuestManager.h"

QuestManager::QuestManager(CYIString questFilePath)
{
    LoadQuestsFromJSON(questFilePath);
}

QuestManager::~QuestManager()
{
    delete m_pQuests;
}

void QuestManager::ActivateCondition(CYIString condition)
{
    m_pQuests->ActivateCondition(condition);
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
}

void QuestManager::LoadQuestsFromJSON(CYIString path)
{
    m_pQuests = QuestList::FromJSON(path);
}