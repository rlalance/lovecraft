#ifndef _TRANSITION_CONTROLLER_H_
#define _TRANSITION_CONTROLLER_H_

#include "screen/NavigationInterface.h"

#include <input/YiBackButtonHandler.h>
#include <screen/YiActivityIndicatorViewController.h>
#include <screen/YiScreenTransition.h>
#include <screen/YiScreenTransitionManager.h>

class ScreenViewController;

class CYIApp;

class NavigationController : public NavigationInterface, public CYIBackButtonHandler::Listener, protected CYIScreenTransitionManager::ScreenTransitionDelegate
{
    struct NavigationRequest
    {
        NavigationRequest(SCREEN_ID e_screen_id, REASON e_reason, const CYIBundle& bundle)
            : eScreenId(e_screen_id),
              eReason(e_reason),
              bundle(bundle)
        {
        }

        const SCREEN_ID eScreenId;
        const REASON eReason;
        const CYIBundle bundle;
    };

public:
    NavigationController(CYIApp &rApp);
    virtual ~NavigationController();

    void Start(SCREEN_ID eScreenId);

    virtual void NavigateToScreen(SCREEN_ID eScreenId, REASON eReason, const CYIBundle &rBundle);
    virtual void NavigateBack();
    bool IsScreenAvailable() const;

private:
    ScreenViewController *CreateScreenViewController(SCREEN_ID eScreenId);
    std::vector<CYIScreenTransitionManager::ScreenTransitionBundle> BuildScreenHistoryFor(ScreenViewController *pScreenViewController);

    virtual bool OnBackButtonPressed();

    virtual void onSurfaceChangeRequested(CYIScreenTransitionManager *pSTM, YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility);
    
    virtual void onSurfaceReady(CYIScreenTransitionManager *pSTM, YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility);

    virtual void OnPopScreen(CYIScreenTransitionManager *pScreenTransitionManager, CYIAbstractScreenViewController *pScreenViewController);

    void UpdateApplicationSurface(YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility);

    CYIApp &m_rApp;
    CYIActivityIndicatorViewController m_ActivityIndicator;
    CYIScreenTransition m_ScreenTransition;

    bool m_bIsStarted;

    std::queue<NavigationRequest> m_NavigationRequests;
    std::list<ScreenViewController *> m_ScreenViewControllers;
    CYIScreenTransitionManager m_ScreenTransitionManager;

    YI_DISALLOW_COPY_AND_ASSIGN(NavigationController);
};

#endif // _TRANSITION_CONTROLLER_H_

