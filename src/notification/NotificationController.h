#ifndef _NOTIFICATION_CONTROLLER_H_
#define _NOTIFICATION_CONTROLLER_H_

#include "notification/NotificationModel.h"

#include <input/YiBackButtonHandler.h>
#include <signal/YiSignalHandler.h>
#include <signal/YiSignal.h>
#include <utility/YiTimer.h>

class Notification;
class NotificationView;
class FullScreenNotificationView;
class PopupNotificationView;

class CYIApp;
class CYISceneView;

class NotificationController : public CYISignalHandler, public CYIBackButtonHandler::Listener
{
public:
    NotificationController(CYIApp &rApp);
    virtual ~NotificationController();

    void Init();
    void NotifyQuestAvailable();
    void NotifyQuestAccepted();
    void NotifyObjectiveCompleted();
    void NotifyQuestCompleted();
private:
    void LoadNotificationOverlays();
    FullScreenNotificationView *LoadDialogOverlay(const CYIString &sceneName, const CYIString &layoutName);
    PopupNotificationView *LoadPopupOverlay(const CYIString &sceneName, const CYIString &layoutName);

    void ShowNextNotification();
    bool IsAnOverlayShown() const;
    void ShowOverlayForDialog(FullScreenNotificationView *pDialogOverlay, const CYISharedPtr<Notification> &pNotification);
    void ShowOverlayForPopup(PopupNotificationView *pDialogOverlay, const CYISharedPtr<Notification> &pNotification);
    void HideOverlay(NotificationView *pDialogOverlay);
    void OnAnimationOutEnded(NotificationView *pNotificationOverlay);

    void NotificationMessageReceived(CYISharedPtr<Notification> pNotification);
    void OverlayAction(FullScreenNotificationView *pDialogOverlay);
    void OverlayDismiss(NotificationView *pDialogOverlay);
    void CloseNotification();

    virtual bool OnBackButtonPressed();
    void OnDeepLinkQueued();

    CYIApp &m_app;
    CYISharedPtr<NotificationModel> m_pNotificationModel;
    PopupNotificationView *m_pPopupErrorLongOverlayView;
    PopupNotificationView *m_pPopupSuccessShortOverlayView;
    FullScreenNotificationView *m_pFullScreenOverlayView;
    std::deque<CYISharedPtr<Notification> > m_notifications;
    CYITimer m_notificationDelayTimer;

    YI_DISALLOW_COPY_AND_ASSIGN(NotificationController);
};

#endif // _NOTIFICATION_CONTROLLER_H_

