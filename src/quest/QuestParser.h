#ifndef QUEST_PARSER_H
#define QUEST_PARSER_H

#include "QuestModel.h"

class QuestParser
{
public:
    static QuestModel* ParseQuest(const CYIString &filepath);
};
#endif
