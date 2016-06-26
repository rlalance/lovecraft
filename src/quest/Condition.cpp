#include "Condition.h"

Condition::Condition(CYIString condition)
{
    m_condition = condition;
    m_bFulfilled = false;
}

CYIString Condition::GetCondition() const
{
    return m_condition;
}

bool Condition::IsFulfilled() const
{
    return m_bFulfilled;
}

void Condition::Trigger()
{
    m_bFulfilled = true;
}

void Condition::Trigger(CYIString condition)
{
    if (condition == m_condition)
    {
        Trigger();
    }
}

CYIString Condition::ToString() const 
{
    CYIString stateString = m_bFulfilled ? "True" : "False";
    return m_condition + ": " + stateString;
}
