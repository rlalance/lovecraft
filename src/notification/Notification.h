#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

#include "messages/Message.h"
#include "notification/NotificationModel.h"

#include <signal/YiSignal.h>
#include <smartptr/YiScopedPtr.h>

class NotificationController;

class Notification
{
    friend class NotificationController;

public:

    /*!
        \details Constructs an error of the given type.
    */
    Notification(NOTIFICATION eNotification);

    /*!
        \details Sets the model from which error properties are loaded.
    */
    static void SetModel(const CYISharedPtr<NotificationModel> &pNotificationModel);

    /*!
        \details Returns the notification identifier.
    */
    NOTIFICATION GetNotificationId() const;
    
    /*!
     \details Returns the type of message.
     */
    NOTIFICATION_TYPE GetType() const;

    /*!
        \details Returns the localized notification title.
    */
    CYIString GetTitle() const;

    /*!
        \details Returns the localized notification message.
    */
    CYIString GetMessage() const;

    /*!
     \details Set the custom title field if supported.
     */
    void SetCustomTitleField(const CYIString &customTitleField);

    /*!
     \details Set the custom message field if supported.
     */
    void SetCustomMessageField(const CYIString &customMessageField);
    
    /*!
     \details Returns the action text.
     */
    CYIString GetAction() const;
    
    /*!
     \details Returns the dismiss.
     */
    CYIString GetDismiss() const;

    /*!
     \details Returns if the popup should allow auto-dismiss.
     */
    bool ShouldAllowAutoDismiss() const;
    
    /*!
     \details Triggers an action on the notification if it is shown.
     */
    void RequestAction();
    
    /*!
        \details Triggers a dismiss on the notification if it is shown.
    */
    void RequestDismiss();

    /*!
     \details Configures the given delegate to be called when an action request is made for this error.
     */
    template<typename Type>
    void DelegateActionRequestTo(Type &object, void (Type::*pFunction)());

    /*!
        \details Configures the given delegate to be called when a dismiss request is made for this error.
    */
    
    template<typename Type>
    void DelegateDismissRequestTo(Type &object, void (Type::*pFunction)());
    
    /*!
     \details Configures the given delegate to be called when the error calls for an action.
     */
    template<typename Type>
    void DelegateActionTo(Type &object, void (Type::*pFunction)());

    /*!
        \details Configures the given delegate to be called when the error calls for a dismiss.
    */
    template<typename Type>
    void DelegateDismissTo(Type &object, void (Type::*pFunction)());

private:
    CYIAny GetDataFromModel(NotificationModel::FIELD eField) const;

    void DelegateAction();
    void DelegateDismiss();

    static CYISharedPtr<NotificationModel> s_pModel;

    const NOTIFICATION m_eNotification;

    CYIString m_customTitleField;
    CYIString m_customMessageField;

    CYISharedPtr<CYISignal<> > m_actionRequestDelegate;
    CYISharedPtr<CYISignal<> > m_dismissRequestDelegate;
    
    CYISharedPtr<CYISignal<> > m_actionDelegate;
    CYISharedPtr<CYISignal<> > m_dismissDelegate;
};

#include "notification/Notification_inl.h"

#endif // _NOTIFICATION_H_

