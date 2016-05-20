#ifndef _TABBED_VIEW_PAGE_H_
#define _TABBED_VIEW_PAGE_H_

#include <view/YiSceneView.h>
#include <smartptr/YiScopedPtr.h>

class TabbedView;

class CYIAbstractTimeline;

class TabbedViewPage : public CYISceneView
{
    friend class TabbedView;

public:

    TabbedViewPage();
    virtual ~TabbedViewPage();

    virtual bool Init();

    bool IsDefault() const;
    CYIString GetPageName() const;
    CYIString GetControllerName() const;
    YI_INT32 GetPageIndex() const;

    void ShowPage();
    
    void TransitionOut(YI_FLOAT fValue);
    void TransitionIn(YI_FLOAT fValue);

    CYISignal<TabbedViewPage *> PageShown;
    CYISignal<TabbedViewPage *> PageHidden;

private:
    void ShowFromLeft(bool bAnimate = true);
    void ShowFromRight(bool bAnimate = true);
    void HideToLeft(bool bAnimate = true);
    void HideToRight(bool bAnimate = true);

    void ResetToShown(bool bShown);
    void ResetToShown();
    void ResetToHidden();

    void OnAnimatedIn();
    void OnAnimatedOut();
    
    CYIScopedPtr<CYIAbstractTimeline> m_pLeftInTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pLeftOutTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pRightInTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pRightOutTimeline;
    
    CYIScopedPtr<CYIAbstractTimeline> m_pLeftInLinearTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pLeftOutLinearTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pRightInLinearTimeline;
    CYIScopedPtr<CYIAbstractTimeline> m_pRightOutLinearTimeline;

    TabbedView *m_pTabbedView;

    YI_DISALLOW_COPY_AND_ASSIGN(TabbedViewPage)

    YI_TYPE_DATA
};

#endif // _TABBED_VIEW_PAGE_H_

