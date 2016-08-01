#include "notification/Notification.h"

#include "notification/NotificationModel.h"

#include <utility/YiString.h>

static const CYIString TAG = "Notification";

CYISharedPtr<NotificationModel> Notification::s_pModel;

Notification::Notification(NOTIFICATION eNotification)
    : m_eNotification(eNotification)
    , m_actionRequestDelegate(new CYISignal<>)
    , m_dismissRequestDelegate(new CYISignal<>)
{
}

void Notification::SetModel(const CYISharedPtr<NotificationModel> &pNotificationModel)
{
    s_pModel = pNotificationModel;
}

NOTIFICATION Notification::GetNotificationId() const
{
    return m_eNotification;
}

NOTIFICATION_TYPE Notification::GetType() const
{
    const CYIString type = GetDataFromModel(NotificationModel::TYPE).Get<CYIString>();
    if (type == "Dialog")
    {
        return DIALOG_NOTIFICATION_TYPE;
    }
    else if (type == "Success")
    {
        return SUCCESS_NOTIFICATION_TYPE;
    }
    else if (type == "Error")
    {
        return ERROR_NOTIFICATION_TYPE;
    }
    return UNKNOWN_NOTIFICATION_TYPE;
}

CYIAny Notification::GetDataFromModel(NotificationModel::FIELD eField) const
{
    YI_ASSERT(s_pModel, TAG, "Model not initialized");

    return s_pModel->GetData(m_eNotification, eField);
}

CYIString Notification::GetTitle() const
{
    return GetDataFromModel(NotificationModel::TITLE).Get<CYIString>().Arg(m_customTitleField);
}

void Notification::SetCustomTitleField(const CYIString &customTitleField)
{
    m_customTitleField = customTitleField;
}

void Notification::SetCustomMessageField(const CYIString &customMessageField)
{
    m_customMessageField = customMessageField;
}

CYIString Notification::GetMessage() const
{
    return GetDataFromModel(NotificationModel::MESSAGE).Get<CYIString>().Arg(m_customMessageField);
}

CYIString Notification::GetAction() const
{
    return GetDataFromModel(NotificationModel::ACTION).Get<CYIString>();
}

CYIString Notification::GetDismiss() const
{
    return GetDataFromModel(NotificationModel::DISMISS).Get<CYIString>();
}

bool Notification::ShouldAllowAutoDismiss() const
{
    const CYIAny &autoDismiss = GetDataFromModel(NotificationModel::AUTODISMISS);

    if (!autoDismiss.Empty())
    {
        return autoDismiss.Get<bool>();
    }

    // Default is to allow auto dismiss
    return true;
}

void Notification::RequestAction()
{
    m_actionRequestDelegate->Emit();
}

void Notification::RequestDismiss()
{
    m_dismissRequestDelegate->Emit();
}

void Notification::DelegateAction()
{
    if (m_actionDelegate)
    {
        (*m_actionDelegate)();
    }
}

void Notification::DelegateDismiss()
{
    if (m_dismissDelegate)
    {
        (*m_dismissDelegate)();
    }
}
