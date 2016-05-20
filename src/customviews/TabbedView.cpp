#include "customviews/TabbedView.h"

#include "customviews/TabbedViewPage.h"
#include "utilities/TimelineUtilities.h"
#include "utilities/NodeUtilities.h"

#include <animation/YiAnimationToggleController.h>
#include <asset/YiAssetViewTemplate.h>
#include <event/YiActionEvent.h>
#include <import/YiViewTemplate.h>
#include <scenetree/YiSceneManager.h>
#include <utility/YiTimer.h>

static const CYIString TAG("TabbedView");

YI_RTTI_DEF1_INST(TabbedView, "TabbedView", CYISceneView)

static const YI_FLOAT s_nScrollThresholdScreenPercentage = 0.03f; // Specifies the horizontal scroll threshold by a percentage amount of the page width
static const YI_INT64 s_iSwipeInputGateResetVelocityRange = 50; // Specifies the velocity range at which a new swipe is allowed to override a previous one

TabbedView::TabbedView()
    : m_bDown(false)
    , m_bIsSwipeMotionSettling(false)
    , m_bIsCurrentSwipeAnInterruptSwipe(false)
    , m_nState(DEFAULT)
    , m_pCurrentPage(YI_NULL)
    , m_pScrollController(YI_NULL)
    , m_bOutgoingPageTransitioning(false)
    , m_bIncomingPageTransitioning(false)
{
    m_pScrollController = new CYIScrollController();
    m_pScrollController->SetScrollListener(this);
}

TabbedView::~TabbedView()
{
}

void TabbedView::SetupScrollControllerProperties()
{
    YI_FLOAT fPageWidth = GetPageWidth();

    m_pScrollController->SetPageSize(fPageWidth, true, true);
    m_pScrollController->SetMagnetRules(true, 0, fPageWidth, 0, 0 );

    m_pScrollController->SetMaxVelocityCoeff(1.0f);
    m_pScrollController->SetFrictionCoeff(1.0f);
}

bool TabbedView::Init()
{
    if (!CYISceneView::Init())
    {
        return false;
    }

    EnableDefaultHitZone();
    EnableEvents(true);
    
    AddEventListener(YI_ACTION_DOWN,  this);
    AddEventListener(YI_ACTION_DOWN,  this, CAPTURE);
    AddEventListener(YI_ACTION_UP,    this);
    AddEventListener(YI_ACTION_MOVE,  this);
    AddEventListener(YI_ACTION_LEAVE, this);
    AddEventListener(YI_ACTION_OUT,   this);
    
    SetSpecialTypeFlag(YI_SPECIAL_EVENT_TARGET_NOPICK);

    SetupScrollControllerProperties();

    if (CYISceneView *pPageTemplateView = GetNode<CYISceneView>("PageTemplate"))
    {
        m_pPageTemplate = pPageTemplateView->GetViewTemplate();
        pPageTemplateView->RequestDeletion();
    }

    m_pages = Utility::GetIndexedNodes<TabbedViewPage>(this, "Page");

    if (GetPageCount() > 0)
    {
        for (YI_INT32 nPageIndex = 0; nPageIndex < GetPageCount(); ++nPageIndex)
        {
            TabbedViewPage *pPage = GetPageAtIndex(nPageIndex);
            pPage->m_pTabbedView = this;

            if (pPage->IsDefault())
            {
                if (!m_pCurrentPage)
                {
                    SetCurrentPage(pPage);
                }
                else
                {
                    YI_LOGW(TAG, "Mutliple pages are marked as default");
                }
            }

            pPage->ResetToShown(m_pCurrentPage == pPage);
        }

        if (!m_pCurrentPage)
        {
            
            SetCurrentPage(GetPageAtIndex(0));
            m_pCurrentPage->ResetToShown();
        }
        
        for (YI_INT32 nPageIndex = 0; nPageIndex < GetPageCount(); ++nPageIndex)
        {
            TabbedViewPage *pPage = GetPageAtIndex(nPageIndex);
            pPage->PageShown.Connect(*this, &TabbedView::OnPageShown);
            pPage->PageHidden.Connect(*this, &TabbedView::OnPageHidden);
        }
    }
    
    // Calculate the scroll swipe threshold from the desired percentage of the page width threshold.
    m_nSurfaceScaledScrollThreshold = GetPageWidth() * s_nScrollThresholdScreenPercentage;
    
    return true;
}

YI_INT32 TabbedView::GetPageCount() const
{
    return static_cast<YI_INT32>(m_pages.size());
}

YI_INT32 TabbedView::GetCurrentPageIndex() const
{
    return GetPageIndex(m_pCurrentPage);
}

YI_INT32 TabbedView::GetPageIndex(const TabbedViewPage *pPage) const
{
    for (YI_INT32 nPageIndex = 0; nPageIndex < GetPageCount(); ++nPageIndex)
    {
        if (GetPageAtIndex(nPageIndex) == pPage)
        {
            return nPageIndex;
        }
    }
    return -1;
}

TabbedViewPage *TabbedView::AddPage(CYISceneView *pView)
{
    return InsertPage(GetPageCount(), pView);
}

TabbedViewPage *TabbedView::InsertPage(YI_INT32 nPageIndex, CYISceneView *pView)
{
    YI_ASSERT(pView && pView->IsInitialized(), TAG, "Invalid view");
    YI_ASSERT(nPageIndex >= 0 && nPageIndex <= GetPageCount(), TAG, "Invalid page index");

    TabbedViewPage *pPage = CreatePage();
    pPage->m_pTabbedView = this;
    pPage->PageShown.Connect(*this, &TabbedView::OnPageShown);
    pPage->PageHidden.Connect(*this, &TabbedView::OnPageHidden);
    m_pages.insert(m_pages.begin() + nPageIndex, pPage);

    Utility::GetNode<CYISceneNode>(pPage, "ViewContainer")->AddChild(pView);
    AddChild(pPage);

    if (m_pCurrentPage == YI_NULL)
    {
        SetCurrentPage(pPage);
        pPage->ResetToShown();
    }
    else
    {
        SetCurrentPage(m_pCurrentPage);
        pPage->ResetToHidden();
    }

    return pPage;
}

TabbedViewPage *TabbedView::GetCurrentPage() const
{
    return m_pCurrentPage;
}

TabbedViewPage *TabbedView::GetPageAtIndex(YI_INT32 nPageIndex) const
{
    if (nPageIndex >= 0 && nPageIndex < GetPageCount())
    {
        return m_pages.at(nPageIndex);
    }
    return YI_NULL;
}

TabbedViewPage *TabbedView::GetPageWithName(const CYIString &pageName) const
{
    const YI_INT32 nPageCount = GetPageCount();

    for (YI_INT32 nPageIndex = 0; nPageIndex < nPageCount; ++nPageIndex)
    {
        TabbedViewPage *pPage = GetPageAtIndex(nPageIndex);
        if (pPage->GetPageName() == pageName)
        {
            return pPage;
        }
    }

    return YI_NULL;
}

void TabbedView::ShowPageAtIndex(YI_INT32 nPageIndex)
{
    ShowPageAtIndex(nPageIndex, true);
}

void TabbedView::ShowPageAtIndex(YI_INT32 nPageIndex, bool bAnimate)
{
    if (!IsTransitioning() && IsPageIndexValid(nPageIndex))
    {
        if (m_pCurrentPage->GetPageIndex() != nPageIndex)
        {
            m_bOutgoingPageTransitioning = true;
            m_bIncomingPageTransitioning = true;

            TabbedViewPage *pOutgoingPage = m_pCurrentPage;
            TabbedViewPage *pIncomingPage = GetPageAtIndex(nPageIndex);
            pIncomingPage->DrawInFront(pOutgoingPage);

            if (nPageIndex > pOutgoingPage->GetPageIndex())
            {
                pOutgoingPage->HideToLeft(bAnimate);
                pIncomingPage->ShowFromRight(bAnimate);
            }
            else
            {
                pOutgoingPage->HideToRight(bAnimate);
                pIncomingPage->ShowFromLeft(bAnimate);
            }
        }
    }
}

void TabbedView::ShowPageWithName(const CYIString &pageName)
{
    ShowPageWithName(pageName, true);
}

void TabbedView::ShowPageWithName(const CYIString &pageName, bool bAnimate)
{
    if (TabbedViewPage *pPage = GetPageWithName(pageName))
    {
        ShowPageAtIndex(pPage->GetPageIndex(), bAnimate);
    }
}

void TabbedView::OnScrolled(CYIScrollController *pControl, YI_SCROLLED_INFO scrolledInfo)
{
    // Don't do anything if the delta is 0.0, meaning, nothing really moved!
    if(glm::epsilonEqual(scrolledInfo.fDelta, 0.0f, glm::epsilon<YI_FLOAT>()))
    {
        return;
    }

    YI_FLOAT fScrollOffset, fSize;
    m_pScrollController->GetDataRange(&fScrollOffset, &fSize);

    YI_FLOAT fPageWidth = GetPageWidth();

    YI_FLOAT fPageOffset = fmod(fScrollOffset, fPageWidth);
    YI_FLOAT fTimelinePercentage = fPageOffset / fPageWidth;

    for(YI_INT32 i = 0; i < GetPageCount(); ++i)
    {
        GetPageAtIndex(i)->Hide();
    }

    YI_INT32 uCurrentPageId = pControl->GetCurPage();
    m_iDirection = (YI_INT32)glm::sign(scrolledInfo.fDelta);
    
    if (glm::abs(fTimelinePercentage) > 0.5f )
    {
        --uCurrentPageId;
    }

    YI_UINT32 outgoingPageId = uCurrentPageId;
    YI_UINT32 incomingPageId = uCurrentPageId + 1;
    
    TabbedViewPage *pOutgoingPage = GetPageAtIndex(outgoingPageId);
    TabbedViewPage *pIncomingPage = GetPageAtIndex(incomingPageId);
    
    if (pOutgoingPage)
    {
        pOutgoingPage->Show();
        pOutgoingPage->TransitionOut(fTimelinePercentage);
    }
    
    if (pIncomingPage)
    {
        pIncomingPage->DrawInFront(pOutgoingPage);
        pIncomingPage->Show();
        pIncomingPage->TransitionIn(-fTimelinePercentage);
    }
}

void TabbedView::OnScrollEnded(CYIScrollController *)
{
    // If the user is registering a new swipe during an active/settling one this is
    // an interupting swipe and the animation timelines/offsets should not be reset.
    if (!m_bIsCurrentSwipeAnInterruptSwipe)
    {
        YI_FLOAT fScrollOffset, fSize;
        m_pScrollController->GetDataRange(&fScrollOffset, &fSize);
        
        YI_FLOAT fPageWidth = GetPageWidth();
        YI_INT32 currentPageId = (YI_INT32)(glm::abs(fScrollOffset / fPageWidth) + 0.5f);
        YI_INT32 outgoingPageId = glm::clamp(currentPageId + m_iDirection, 0, GetPageCount() - 1);
        
        TabbedViewPage *pOutgoingPage = GetPageAtIndex(outgoingPageId);
        TabbedViewPage *pIncomingPage = GetPageAtIndex(currentPageId);
        
        if(pIncomingPage && pOutgoingPage && pIncomingPage != pOutgoingPage)
        {
            pIncomingPage->PageShown(pIncomingPage);
            pIncomingPage->TransitionOut(0.0f);
            pOutgoingPage->PageHidden(pOutgoingPage);
            pOutgoingPage->TransitionIn(0.0f);
        }
        else
        {
            // We are not transiting to anywhere.
            // Reset the transition flags.
            m_bIncomingPageTransitioning = false;
            m_bOutgoingPageTransitioning = false;
        }
    }
    else
    {
        m_bIsCurrentSwipeAnInterruptSwipe = false;
    }
}

void TabbedView::SetDownState(bool bDownState)
{
    m_bDown = bDownState;
}

void TabbedView::SetScrollState(STATE nState)
{
    m_nState = nState;
}

bool TabbedView::HandleEvent(const CYISharedPtr<CYIEventDispatcher> &, CYIEvent *pEvent)
{
    bool bHandled = false;

    if (pEvent->IsActionEvent())
    {
        CYIActionEvent *pActionEvent = static_cast<CYIActionEvent *>(pEvent);

        CYISceneNode *pCapturingNode = GetSceneManager()->GetPointerCaptureNode(pActionEvent->m_uPointerID);
        if (pCapturingNode && (pCapturingNode != this))
        {
            return false;
        }

        switch (pActionEvent->GetType())
        {
            case YI_ACTION_UP:
            {
                SetDownState(false);
                break;
            }
            case YI_ACTION_DOWN:
            {
                SetDownState(true);
                m_vPointerStartPos = pActionEvent->m_vLocalSpaceLocation;
                m_uScrollDownTime = pEvent->m_uEventTimeMs;
                
                bHandled = true;
                break;
            }
            case YI_ACTION_LEAVE:
            {
                if(m_nState == SCROLLING)
                {
                    // can't rely on event location for the LEAVE event !
                    SetScrollState(DEFAULT);
                    m_pScrollController->GestureEnd(m_vPointerLastPos.x, pEvent->m_uEventTimeMs);
                    StopCapturePointerEvents(pActionEvent->m_uPointerID);
                    m_bIsSwipeMotionSettling = true;
                }
                SetDownState(false);
                bHandled = true;
                break;
            }
            case YI_ACTION_MOVE:
            {
                // Check if there is currently a swipe action being processed.
                if (m_bIsSwipeMotionSettling)
                {
                    YI_INT64 currentScrollVelocity = m_pScrollController->GetCurrentVelocity();
                    if (currentScrollVelocity >= -s_iSwipeInputGateResetVelocityRange && currentScrollVelocity <= s_iSwipeInputGateResetVelocityRange)
                    {
                        m_bIsSwipeMotionSettling = false;
                    }
                }
                
                // Check if this move event is either a continued swipe/scroll motion or a new one.
                m_vPointerLastPos = pActionEvent->m_vLocalSpaceLocation;
                if(m_nState == SCROLLING)
                {
                    // Continue a swipe/scroll motion that has already started.
                    m_pScrollController->GestureMove(m_vPointerLastPos.x, pEvent->m_uEventTimeMs);
                    bHandled = true;
                }
                else
                {
                    // If there is an active swipe settling we want the swipe feedback to be as sensitive as possible for the best
                    // user experience, so skip the X threshold checks when an existing swipe motion is still settling.
                    if (m_bIsSwipeMotionSettling ||
                        (m_bDown && (glm::abs(m_vPointerLastPos.x - m_vPointerStartPos.x) > m_nSurfaceScaledScrollThreshold)))
                    {
                        // Starting a new swipe/scroll motion.
                        SetScrollState(SCROLLING);
                        
                        // If the user is registering a new swipe during an active/settling one, notify for the onScrollEnded() call that this is
                        // an interupting swipe and animation timelines/offsets should not be reset.
                        m_bIsCurrentSwipeAnInterruptSwipe = m_bIsSwipeMotionSettling ? (true) : (false);
                        
                        m_pScrollController->GestureStart(m_vPointerStartPos.x, m_uScrollDownTime);
                        m_pScrollController->GestureMove(m_vPointerLastPos.x, pEvent->m_uEventTimeMs);
                        StartCapturePointerEvents(pActionEvent->m_uPointerID);
                        
                        m_bOutgoingPageTransitioning = true;
                        m_bIncomingPageTransitioning = true;
                        
                        bHandled = true;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    return bHandled;
}

TabbedViewPage *TabbedView::CreatePage() const
{
    YI_ASSERT(m_pPageTemplate, TAG, "Missing page template");

    const CYISharedPtr<CYIViewTemplate> pViewTemplate = m_pPageTemplate->GetTemplate();

    CYIScopedPtr<CYISceneView> pPageView(pViewTemplate->BuildView(GetSceneManager(), m_pPageTemplate));
    YI_ASSERT(pPageView, TAG, "Could not instantiate page view");
    CYIScopedPtr<TabbedViewPage> pPage(YiDynamicCast<TabbedViewPage>(pPageView.Take()));
    YI_ASSERT(pPage, TAG, "Could not instantiate page view");

    if (!pPage->Init())
    {
        YI_ASSERT(false, TAG, "Could not initialize page");

        return YI_NULL;
    }

    return pPage.Take();
}

bool TabbedView::IsTransitioning() const
{
    return m_bIncomingPageTransitioning || m_bOutgoingPageTransitioning;
}

bool TabbedView::IsPageIndexValid(YI_INT32 nPageIndex) const
{
    return nPageIndex >= 0 && nPageIndex < GetPageCount();
}

void TabbedView::OnPageShown(TabbedViewPage *pPage)
{
    m_bIncomingPageTransitioning = false;
    
    SetCurrentPage(pPage);

    PageShown.Emit(pPage);
    PageShownAtIndex.Emit(pPage->GetPageIndex());
}

void TabbedView::OnPageHidden(TabbedViewPage *pPage)
{
    m_bOutgoingPageTransitioning = false;

    PageHidden.Emit(pPage);
    PageHiddenAtIndex.Emit(pPage->GetPageIndex());
}

void TabbedView::SetCurrentPage(TabbedViewPage *pPage)
{
    const YI_INT32 nPageIndex = pPage->GetPageIndex();

    YI_FLOAT fPageWidth = GetPageWidth();

    m_pCurrentPage = pPage;
    m_pScrollController->SetDataRange(-nPageIndex * fPageWidth, fPageWidth * GetPageCount());
    
    for (YI_INT32 nPageIndex = 0; nPageIndex < GetPageCount(); ++nPageIndex)
    {
        TabbedViewPage *pPage = GetPageAtIndex(nPageIndex);
        pPage->Hide();
    }
    
    m_pCurrentPage->Show();
}

YI_FLOAT TabbedView::GetPageWidth()
{
    return GetSize().x;
}

YI_FLOAT TabbedView::GetPageHeight()
{
    return GetSize().y;
}