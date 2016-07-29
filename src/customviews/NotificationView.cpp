#include "customviews/NotificationView.h"

#include "utilities/NodeUtilities.h"
#include "utilities/TimelineUtilities.h"

#include <animation/YiTimelineGroup.h>
#include <scenetree/YiSceneManager.h>
#include <scenetree/YiTextSceneNode.h>

const CYIString TAG("NotificationView");

YI_RTTI_DEF1_INST(NotificationView, "NotificationView", CYISceneView)

NotificationView::NotificationView()
{
}

NotificationView::~NotificationView()
{
}

bool NotificationView::Init()
{
    bool bOK = CYISceneView::Init();
    
    if (bOK)
    {
        CYITimelineGroup *pOffTimeline = TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "Out");
        CYITimelineGroup *pOnTimeline = TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "In");
        
        m_diplayAnimationController.SetTimelines(pOnTimeline, pOffTimeline, CYIAnimationToggleController::SHARED);
        m_diplayAnimationController.ResetToState(CYIAnimationToggleController::OFF);
        
        pOffTimeline->Completed.Connect(*this, &NotificationView::AnimationdOutEnd);
    }
    
    return bOK;
}

void NotificationView::SetSceneName(const CYIString &sceneName)
{
    m_sceneName = sceneName;
}

const CYIString &NotificationView::GetSceneName() const
{
    return m_sceneName;
}

void NotificationView::SetTitle(const CYIString &text)
{
    SetMessageText("Placeholder-Title", text);
}

void NotificationView::SetMessage(const CYIString &text)
{
    SetMessageText("Placeholder-Description", text);
}

void NotificationView::ShowOverlay()
{
    m_diplayAnimationController.ToggleOn();
}

void NotificationView::HideOverlay()
{
    m_diplayAnimationController.ToggleOff();
}

bool NotificationView::IsOverlayShown()
{
    return m_diplayAnimationController.GetState() == CYIAnimationToggleController::ON;
}

void NotificationView::DoDismiss()
{
    Dismissed(this);
}

void NotificationView::AnimationdOutEnd()
{
    AnimationdOutEnded(this);
}

void NotificationView::SetMessageText(const CYIString &textField, const CYIString &text)
{
    CYISceneView *pMessage = Utility::GetNode<CYISceneView>(this, "Message", Utility::GET_NODE_FLAG_OPTIONAL);
    
    if (pMessage != YI_NULL)
    {
        CYITextSceneNode *pMessagePopup = Utility::GetNode<CYITextSceneNode>(pMessage, textField);
        
        if (pMessagePopup != YI_NULL)
        {
            pMessagePopup->SetText(text);
        }
    }
}

