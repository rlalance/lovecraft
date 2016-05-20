#include "customviews/TabbedViewPage.h"

#include "customviews/TabbedView.h"
#include "utilities/NodeUtilities.h"
#include "utilities/TimelineUtilities.h"

#include <event/YiActionEvent.h>
#include <scenetree/YiSceneManager.h>

static const CYIString TAG("TabbedViewPage");

/*!
    \details Takes to completion pTimeline either by playing 
    it or by seeking to the end of it based on bAnimate.
*/
static inline void CompleteTimeline(CYIAbstractTimeline* pTimeline, bool bAnimate = true)
{
    if (bAnimate)
    {
        pTimeline->StartForward();
    }
    else
    {
        pTimeline->SeekToPercentage(100);
        pTimeline->Completed.Emit();
    }
};

static inline void Transition(CYIAbstractTimeline* pLeftTimeline, CYIAbstractTimeline* pRightTimeline, YI_FLOAT fValue)
{
    if (fValue > 0)
    {
        pRightTimeline->SeekToPercentage(fValue);
    }
    else
    {
        pLeftTimeline->SeekToPercentage(-fValue);
    }
}

YI_RTTI_DEF1_INST(TabbedViewPage, "TabbedViewPage", CYISceneView)

TabbedViewPage::TabbedViewPage()
    : m_pTabbedView(YI_NULL)
{
}

TabbedViewPage::~TabbedViewPage()
{
}

bool TabbedViewPage::Init()
{
    if (!CYISceneView::Init())
    {
        return false;
    }
    
    EnableDefaultHitZone();
    EnableEvents(true);
    
    // Left in
    m_pLeftInTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "LeftIn"));
    m_pLeftInTimeline->SetDisablesInput(true);
    m_pLeftInTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedIn);

    // Left out
    m_pLeftOutTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "LeftOut"));
    m_pLeftOutTimeline->SetDisablesInput(true);
    m_pLeftOutTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedOut);

    // Right in
    m_pRightInTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "RightIn"));
    m_pRightInTimeline->SetDisablesInput(true);
    m_pRightInTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedIn);

    // Right out
    m_pRightOutTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "RightOut"));
    m_pRightOutTimeline->SetDisablesInput(true);
    m_pRightOutTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedOut);
    
    
    // Left in line
    m_pLeftInLinearTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "LeftIn-Linear"));
    m_pLeftInLinearTimeline->SetDisablesInput(true);
    m_pLeftInLinearTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedIn);
    
    // Left out line
    m_pLeftOutLinearTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "LeftOut-Linear"));
    m_pLeftOutLinearTimeline->SetDisablesInput(true);
    m_pLeftOutLinearTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedOut);
    
    // Right in line
    m_pRightInLinearTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "RightIn-Linear"));
    m_pRightInLinearTimeline->SetDisablesInput(true);
    m_pRightInLinearTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedIn);
    
    // Right out line
    m_pRightOutLinearTimeline.Reset(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(this, "RightOut-Linear"));
    m_pRightOutLinearTimeline->SetDisablesInput(true);
    m_pRightOutLinearTimeline->Completed.Connect(*this, &TabbedViewPage::OnAnimatedOut);

    return true;
}

bool TabbedViewPage::IsDefault() const
{
    bool bIsDefault = false;
    Utility::GetProperty<bool>(this, "default", bIsDefault);
    return bIsDefault;
}

CYIString TabbedViewPage::GetPageName() const
{
    CYIString pageName;
    Utility::GetProperty<CYIString>(this, "name", pageName);
    return pageName;
}

CYIString TabbedViewPage::GetControllerName() const
{
    CYIString controllerName;
    Utility::GetProperty<CYIString>(this, "controller", controllerName);
    return controllerName;
}

YI_INT32 TabbedViewPage::GetPageIndex() const
{
    return m_pTabbedView->GetPageIndex(this);
}

void TabbedViewPage::ShowPage()
{
    YI_ASSERT(m_pTabbedView, TAG, "Invalid tabbed view");
    m_pTabbedView->ShowPageAtIndex(GetPageIndex());
}

void TabbedViewPage::ShowFromLeft(bool bAnimate)
{
    Show();
    CompleteTimeline(m_pLeftInTimeline.Get(), bAnimate);
}

void TabbedViewPage::ShowFromRight(bool bAnimate)
{
    Show();
    CompleteTimeline(m_pRightInTimeline.Get(), bAnimate);
}

void TabbedViewPage::HideToLeft(bool bAnimate)
{
    // Hide() is called in OnAnimatedOut() when the timeline completes
    CompleteTimeline(m_pLeftOutTimeline.Get(), bAnimate);
}

void TabbedViewPage::HideToRight(bool bAnimate)
{
    // Hide() is called in OnAnimatedOut() when the timeline completes
    CompleteTimeline(m_pRightOutTimeline.Get(), bAnimate);
}

void TabbedViewPage::ResetToShown(bool bShown)
{
    if (bShown)
    {
        ResetToShown();
    }
    else
    {
        ResetToHidden();
    }
}

void TabbedViewPage::ResetToShown()
{
    // The direction here don't matter because we aren't
    // animating the show. We're using left but could just
    // as easily have used right.
    ShowFromLeft(false);
}

void TabbedViewPage::ResetToHidden()
{
    // The direction here don't matter because we aren't
    // animating the hide. We're using left but could just
    // as easily have used right.
    HideToLeft(false);
}

void TabbedViewPage::OnAnimatedIn()
{
    PageShown.Emit(this);
}

void TabbedViewPage::OnAnimatedOut()
{
    Hide();
    PageHidden.Emit(this);
}

void TabbedViewPage::TransitionIn(YI_FLOAT fValue)
{
    Transition(m_pLeftInLinearTimeline.Get(), m_pRightInLinearTimeline.Get(), fValue);
}

void TabbedViewPage::TransitionOut(YI_FLOAT fValue)
{
    Transition(m_pLeftOutLinearTimeline.Get(), m_pRightOutLinearTimeline.Get(), fValue);
}
