#ifndef _TABBED_VIEW_H_
#define _TABBED_VIEW_H_

#include <utility/YiCondition.h>
#include <view/YiSceneView.h>
#include <view/YiScrollController.h>
#include <smartptr/YiScopedPtr.h>

class CYIScrollController;
class CYIAssetViewTemplate;

class TabbedViewPage;

/*
 * Tabbed view requires pages of identical sizes due to the way we use of the scroll controller.
 */
class TabbedView : public CYISceneView, public CYIScrollController::ScrollListener
{
    friend class TabbedViewPage;

public:
    TabbedView();
    virtual ~TabbedView();

    virtual bool Init();

    YI_INT32 GetPageCount() const;
    YI_INT32 GetCurrentPageIndex() const;
    YI_INT32 GetPageIndex(const TabbedViewPage *pPage) const;

    TabbedViewPage *AddPage(CYISceneView *pView);
    TabbedViewPage *InsertPage(YI_INT32 nPageIndex, CYISceneView *pView);
    TabbedViewPage *GetCurrentPage() const;
    TabbedViewPage *GetPageAtIndex(YI_INT32 nPageIndex) const;
    TabbedViewPage *GetPageWithName(const CYIString &pageName) const;
    YI_FLOAT GetPageWidth();
    YI_FLOAT GetPageHeight();

    void ShowPageAtIndex(YI_INT32 nPageIndex);
    void ShowPageAtIndex(YI_INT32 nPageIndex, bool bAnimate);
    void ShowPageWithName(const CYIString &pageName);
    void ShowPageWithName(const CYIString &pageName, bool bAnimate);

    CYISignal<TabbedViewPage *> PageShown;
    CYISignal<TabbedViewPage *> PageHidden;
    CYISignal<YI_INT32> PageShownAtIndex;
    CYISignal<YI_INT32> PageHiddenAtIndex;

protected:
    virtual bool HandleEvent(const CYISharedPtr<CYIEventDispatcher> &pDispatcher, CYIEvent *pEvent);
    virtual void OnScrolled(CYIScrollController *pControl, YI_SCROLLED_INFO scrolledInfo);
    virtual void OnScrollEnded(CYIScrollController *pControl);

private:
    enum STATE
    {
        DEFAULT,
        SCROLLING
    };

    TabbedViewPage *CreatePage() const;

    bool IsTransitioning() const;
    bool IsPageIndexValid(YI_INT32 nPageIndex) const;

    void OnPageShown(TabbedViewPage *pPage);
    void OnPageHidden(TabbedViewPage *pPage);
    
    void SetCurrentPage(TabbedViewPage *pPage);
    
    void SetDownState(bool bDownState);
    void SetScrollState(STATE nState);
    void SetupScrollControllerProperties();
    
    bool m_bDown;
    bool m_bIsSwipeMotionSettling;
    bool m_bIsCurrentSwipeAnInterruptSwipe;
    
    STATE m_nState;
    
    YI_FLOAT m_nSurfaceScaledScrollThreshold;
    YI_INT64 m_uScrollDownTime;
    YI_INT32 m_iDirection;
    
    glm::vec3 m_vPointerStartPos;
    glm::vec3 m_vPointerLastPos;
    
    std::vector<TabbedViewPage *> m_pages;
    TabbedViewPage *m_pCurrentPage;
    CYIScopedPtr<CYIScrollController> m_pScrollController;

    bool m_bOutgoingPageTransitioning;
    bool m_bIncomingPageTransitioning;

    CYISharedPtr<CYIAssetViewTemplate> m_pPageTemplate;

    YI_DISALLOW_COPY_AND_ASSIGN(TabbedView)

    YI_TYPE_DATA
};

#endif // _TABBED_VIEW_H_
