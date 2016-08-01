#include "notification/NotificationController.h"

#include "customviews/FullScreenNotificationView.h"
#include "customviews/PopupNotificationView.h"
#include "notification/Notification.h"
#include "utilities/SceneLayers.h"

#include <framework/YiApp.h>
#include <scenetree/YiSceneManager.h>
#include <utility/YiTranslation.h>
#include <quest/QuestManager.h>

static const CYIString TAG("NotificationController");

static const YI_CHAR *const FULLSCREEN_ERROR_OVERLAY_SCENE_NAME = "FullscreenErrorOverlaySceneName";
static const YI_CHAR *const POPUP_ERROR_LONG_OVERLAY_SCENE_NAME = "PopupErrorLongOverlaySceneName";
static const YI_CHAR *const POPUP_SUCCESS_SHORT_OVERLAY_SCENE_NAME = "PopupSuccessShortOverlaySceneName";
static const YI_INT32 NOTIFICATION_DELAY_MS = 3000;

static inline CYIString GetErrorJSONPath(CYIApp &app)
{
    return app.GetAssetsPath() + "text/Notifications.json";
}

NotificationController::NotificationController(CYIApp &app)
    : m_app(app)
    , m_pNotificationModel(new NotificationModel(GetErrorJSONPath(app)))
    , m_pPopupErrorLongOverlayView(YI_NULL)
    , m_pPopupSuccessShortOverlayView(YI_NULL)
    , m_pFullScreenOverlayView(YI_NULL)
{
}

NotificationController::~NotificationController()
{
}

void NotificationController::Init()
{
    LoadNotificationOverlays();

    m_pNotificationModel->Fetch();
    Notification::SetModel(m_pNotificationModel);
    Message<Notification>::Subscribe(*this, &NotificationController::NotificationMessageReceived);

    m_notificationDelayTimer.SetInterval(NOTIFICATION_DELAY_MS);
    m_notificationDelayTimer.TimedOut.Connect(*this, &NotificationController::CloseNotification);
    
    QuestManager::QuestAvailableSig.Connect(*this, &NotificationController::NotifyQuestAvailable);
    QuestManager::QuestAcceptedSig.Connect(*this, &NotificationController::NotifyQuestAccepted);
    QuestManager::ObjectiveCompletedSig.Connect(*this, &NotificationController::NotifyObjectiveCompleted);
    QuestManager::QuestCompletedSig.Connect(*this, &NotificationController::NotifyQuestCompleted);
}

void NotificationController::NotifyQuestAvailable(QuestModel *pQuestModel)
{
    Notification questAvailableNotification(QUEST_AVAILABLE);
    questAvailableNotification.SetCustomMessageField(pQuestModel->GetDisplayText());

    Message<Notification>(questAvailableNotification).Send();
}

void NotificationController::NotifyQuestAccepted(QuestModel *pQuestModel)
{
    Notification questAvailableNotification(QUEST_AVAILABLE);
    questAvailableNotification.SetCustomMessageField(pQuestModel->GetDisplayText());

    Message<Notification>(questAvailableNotification).Send();
}

void NotificationController::NotifyObjectiveCompleted(QuestObjectiveModel *pQuestObjectiveModel)
{
    Notification questAvailableNotification(QUEST_AVAILABLE);
    questAvailableNotification.SetCustomMessageField(pQuestObjectiveModel->GetDisplayText());

    Message<Notification>(questAvailableNotification).Send();
}

void NotificationController::NotifyQuestCompleted(QuestModel *pQuestModel)
{
    Notification questAvailableNotification(QUEST_AVAILABLE);
    questAvailableNotification.SetCustomMessageField(pQuestModel->GetDisplayText());

    Message<Notification>(questAvailableNotification).Send();
}

void NotificationController::LoadNotificationOverlays()
{
    m_pFullScreenOverlayView = LoadDialogOverlay(FULLSCREEN_ERROR_OVERLAY_SCENE_NAME, "Notifications_MainComp-Fullscreen-Error.layout");
    m_pPopupErrorLongOverlayView = LoadPopupOverlay(POPUP_ERROR_LONG_OVERLAY_SCENE_NAME, "Notifications_MainComp-Popup-Error-Long.layout");
    m_pPopupSuccessShortOverlayView = LoadPopupOverlay(POPUP_SUCCESS_SHORT_OVERLAY_SCENE_NAME, "Notifications_MainComp-Popup-Success-Short.layout");
}

FullScreenNotificationView *NotificationController::LoadDialogOverlay(const CYIString &sceneName, const CYIString &layoutName)
{
    CYISceneManager *pSceneManager = m_app.GetSceneManager();
    
    CYISceneView *pSceneView = pSceneManager->LoadScene(layoutName, CYISceneManager::SCALE_RESPONSIVE_LAYOUT, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);
    YI_ASSERT(pSceneView, TAG, "Scene manager failed to load '%s'", layoutName.GetData());
    
    FullScreenNotificationView *pDialogOverlay = static_cast<FullScreenNotificationView*>(pSceneView);
    pDialogOverlay->Actioned.Connect(*this, &NotificationController::OverlayAction);
    pDialogOverlay->Dismissed.Connect(*this, &NotificationController::OverlayDismiss);
    pDialogOverlay->AnimationdOutEnded.Connect(*this,&NotificationController::OnAnimationOutEnded);
    pDialogOverlay->ConnectButtons();
    
    pSceneManager->AddScene(sceneName, pDialogOverlay, SceneLayers::LAYER_NOTIFICATIONS, CYISceneManager::LAYER_TRANSPARENT);
    pSceneManager->UnstageScene(sceneName);
    pDialogOverlay->SetSceneName(sceneName);
    
    return pDialogOverlay;
}

PopupNotificationView *NotificationController::LoadPopupOverlay(const CYIString &sceneName, const CYIString &layoutName)
{
    CYISceneManager *pSceneManager = m_app.GetSceneManager();
    
    CYISceneView *pSceneView = pSceneManager->LoadScene(layoutName, CYISceneManager::SCALE_RESPONSIVE_LAYOUT, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);
    YI_ASSERT(pSceneView, TAG, "Scene manager failed to load '%s'", layoutName.GetData());
    
    PopupNotificationView *pPopupOverlay = static_cast<PopupNotificationView*>(pSceneView);
    pPopupOverlay->Dismissed.Connect(*this, &NotificationController::OverlayDismiss);
    pPopupOverlay->AnimationdOutEnded.Connect(*this,&NotificationController::OnAnimationOutEnded);
    
    pSceneManager->AddScene(sceneName, pPopupOverlay, SceneLayers::LAYER_NOTIFICATIONS, CYISceneManager::LAYER_TRANSPARENT);
    pSceneManager->UnstageScene(sceneName);
    pPopupOverlay->SetSceneName(sceneName);
    
    return pPopupOverlay;
}

void NotificationController::NotificationMessageReceived(CYISharedPtr<Notification> pNotification)
{
    std::deque<CYISharedPtr<Notification> >::const_iterator iter = m_notifications.begin();
    std::deque<CYISharedPtr<Notification> >::const_iterator end = m_notifications.end();

    // Ignore the error if it has already been queued for display.
    for (; iter != end; ++iter)
    {
        if ((*iter)->GetNotificationId() == pNotification->GetNotificationId())
        {
            return;
        }
    }

    if (m_notifications.empty())
    {
        CYIBackButtonHandler::AddListener(this);
    }
    
    m_notifications.push_back(pNotification);

    ShowNextNotification();
}

void NotificationController::ShowNextNotification()
{
    if (!IsAnOverlayShown() && m_notifications.size() > 0)
    {
        CYISharedPtr<Notification> pNotification = m_notifications.front();

        switch (pNotification->GetType()) {
            case DIALOG_NOTIFICATION_TYPE:
                ShowOverlayForDialog(m_pFullScreenOverlayView, pNotification);
                break;
                
            case SUCCESS_NOTIFICATION_TYPE:
                ShowOverlayForPopup(m_pPopupSuccessShortOverlayView, pNotification);
                break;
                
            case ERROR_NOTIFICATION_TYPE:
                ShowOverlayForPopup(m_pPopupErrorLongOverlayView, pNotification);
                break;
            
            case GENERAL_NOTIFICATION_TYPE:
            case UNKNOWN_NOTIFICATION_TYPE:
            default:
                break;
        }
    }
    else
    {
        CYIBackButtonHandler::RemoveListener(this);
    }
}

bool NotificationController::IsAnOverlayShown() const
{
    return m_pFullScreenOverlayView->IsOverlayShown() || m_pPopupErrorLongOverlayView->IsOverlayShown() || m_pPopupSuccessShortOverlayView->IsOverlayShown();
}

void NotificationController::ShowOverlayForDialog(FullScreenNotificationView *pDialogOverlay, const CYISharedPtr<Notification> &pNotification)
{
    pDialogOverlay->SetTitle(pNotification->GetTitle());
    pDialogOverlay->SetMessage(pNotification->GetMessage());
    pDialogOverlay->SetActionButtonText(pNotification->GetAction());
    pDialogOverlay->SetDismissButtonText(pNotification->GetDismiss());

    pNotification->DelegateActionRequestTo(*pDialogOverlay, &FullScreenNotificationView::DoAction);
    pNotification->DelegateDismissRequestTo(*pDialogOverlay, &FullScreenNotificationView::DoDismiss);

    m_app.GetSceneManager()->StageScene(pDialogOverlay->GetSceneName());
    pDialogOverlay->ShowOverlay();
}

void NotificationController::ShowOverlayForPopup(PopupNotificationView *pPopupOverlay, const CYISharedPtr<Notification> &pNotification)
{
    pPopupOverlay->SetShortTitle(pNotification->GetTitle());
    pPopupOverlay->SetMessage(pNotification->GetMessage());
    pPopupOverlay->ConnectButtons();
    pNotification->DelegateDismissRequestTo(*pPopupOverlay, &PopupNotificationView::DoDismiss);

    m_app.GetSceneManager()->StageScene(pPopupOverlay->GetSceneName());
    pPopupOverlay->ShowOverlay();

    if(pNotification->ShouldAllowAutoDismiss())
    {
         m_notificationDelayTimer.Start();
    }
}

void NotificationController::HideOverlay(NotificationView *pNotificationOverlay)
{
    pNotificationOverlay->HideOverlay();
}

void NotificationController::OnAnimationOutEnded(NotificationView *pNotificationOverlay)
{
    m_app.GetSceneManager()->UnstageScene(pNotificationOverlay->GetSceneName());
}

void NotificationController::OverlayAction(FullScreenNotificationView *pErrorOverlay)
{
    HideOverlay(pErrorOverlay);
    
    if (m_notifications.size())
    {
        m_notifications.front()->DelegateAction();
        m_notifications.pop_front();
    }
    
    ShowNextNotification();
}

void NotificationController::OverlayDismiss(NotificationView *pErrorOverlay)
{
    HideOverlay(pErrorOverlay);
    
    if (m_notifications.size())
    {
        m_notifications.front()->DelegateDismiss();
        m_notifications.pop_front();
    }

    ShowNextNotification();
}

void NotificationController::CloseNotification()
{
    m_notificationDelayTimer.Stop();
    
    if (m_pPopupSuccessShortOverlayView->IsOverlayShown())
    {
        OverlayDismiss(m_pPopupSuccessShortOverlayView);
    }
    else if (m_pPopupErrorLongOverlayView->IsOverlayShown())
    {
        OverlayDismiss(m_pPopupErrorLongOverlayView);
    }
}

bool NotificationController::OnBackButtonPressed()
{
    bool bHandled = false;

    if (m_pFullScreenOverlayView->IsOverlayShown())
    {
        OverlayDismiss(m_pFullScreenOverlayView);
        bHandled = true;
    }
    else if (m_pPopupSuccessShortOverlayView->IsOverlayShown())
    {
        OverlayDismiss(m_pPopupSuccessShortOverlayView);
        bHandled = true;
    }
    else if (m_pPopupErrorLongOverlayView->IsOverlayShown())
    {
        OverlayDismiss(m_pPopupErrorLongOverlayView);
        bHandled = true;
    }

    return bHandled;
}

void NotificationController::OnDeepLinkQueued()
{
    if (!m_notifications.empty() && (m_notifications.front()->GetNotificationId() != APP_CONFIGURATION_ERROR))
    {
        OverlayDismiss(m_pFullScreenOverlayView);
    }
}
