#ifndef _NOTIFICATION_INL_H_
#define _NOTIFICATION_INL_H_

template<typename Type>
void Notification::DelegateActionRequestTo(Type &object, void (Type::*pFunction)())
{
    m_actionRequestDelegate->Connect(object, pFunction);
}

template<typename Type>
void Notification::DelegateDismissRequestTo(Type &object, void (Type::*pFunction)())
{
    m_dismissRequestDelegate->Connect(object, pFunction);
}

template<typename Type>
void Notification::DelegateActionTo(Type &object, void (Type::*pFunction)())
{
    if (!m_actionDelegate)
    {
        m_actionDelegate = CYISharedPtr<CYISignal<> >(new CYISignal<>);
    }
    m_actionDelegate->Connect(object, pFunction);
}

template<typename Type>
void Notification::DelegateDismissTo(Type &object, void (Type::*pFunction)())
{
    if (!m_dismissDelegate)
    {
        m_dismissDelegate = CYISharedPtr<CYISignal<> >(new CYISignal<>);
    }
    m_dismissDelegate->Connect(object, pFunction);
}

#endif // _NOTIFICATION_INL_H_
