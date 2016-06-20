#ifndef CONDITION_H
#define CONDITION_H

#include <utility/YiString.h>
#include <rapidjson/document.h>

class Condition
{
public:
    Condition(CYIString condition);
    CYIString GetCondition() const;
    bool IsFulfilled() const;
    void Trigger();
    void Trigger(CYIString condition);

    CYIString ToString() const;
private:
    CYIString m_condition;
    bool m_bFulfilled;
};

#endif
