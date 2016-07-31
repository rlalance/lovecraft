#include "BookScreenViewController.h"

#include "customviews/TabbedView.h"
#include "screen/TabbedViewController.h"
#include "utilities/NodeUtilities.h"

#include <animation/YiAbstractTimeline.h>
#include <framework/YiApp.h>
#include <framework/YiFramework.h>
#include <view/YiPushButtonView.h>
#include <messages/Message.h>
#include <notification/Notification.h>

BookScreenViewController::BookScreenViewController(NavigationInterface &navigationInterface, CYIApp &app) :
TabbedScreenViewController(NavigationInterface::SPLASH, "BookScreen", "Book_Main.layout", navigationInterface, app)
, m_tabbedViewController(navigationInterface)
, m_pTabbedView(YI_NULL)
, m_pButtonBarView(YI_NULL)
{
}

BookScreenViewController::~BookScreenViewController()
{
}

void BookScreenViewController::BuildTimelines()
{
    m_pInTimeline = m_pView->BuildTimelineGroup("In");
    if (m_pInTimeline)
    {
        SetBeginAnimation(m_pInTimeline.Get(), TimelineHelper::ANIMATE_FORWARD);
    }

    m_pOutTimeline = m_pView->BuildTimelineGroup("Out");
    if (m_pOutTimeline)
    {
        SetEndAnimation(m_pOutTimeline.Get(), TimelineHelper::ANIMATE_FORWARD);
    }
}

void BookScreenViewController::OnInitScreen()
{
    TabbedScreenViewController::OnInitScreen();

    m_pButtonBarView = Utility::GetNode<ButtonBarView>(m_pView, "ButtonBar", Utility::GET_NODE_FLAG_OPTIONAL);
    m_pTabbedView = Utility::GetNode<TabbedView>(m_pView, "TabbedView", Utility::GET_NODE_FLAG_OPTIONAL);

    if (m_pTabbedView)
    {
        m_tabbedViewController.Init(m_pTabbedView);
    }
}

void BookScreenViewController::OnPreloadValidateScreen(CYIBundle)
{
    PreloadValidateSucceeded.Emit(this);
}

void BookScreenViewController::OnPreloadScreen(CYIBundle bundle, const CYIPersistentStore &stateData)
{
    TabbedScreenViewController::OnPreloadScreen(bundle, stateData);

    PreloadFinished(this);
}

