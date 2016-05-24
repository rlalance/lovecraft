#ifndef _TABBED_SCREEN_VIEW_CONTROLLER_H_
#define _TABBED_SCREEN_VIEW_CONTROLLER_H_

#include "screen/ScreenViewController.h"

#include "screen/TabbedViewController.h"

/*!
    \details TabbedScreenViewController is a base class for screens with
    tabbed views. It handles scene set up and tabbed view and tabbed
    view controller management common to all screens with tabbed views.
*/
class TabbedScreenViewController : public ScreenViewController
{

public:
    TabbedScreenViewController(NavigationInterface::SCREEN_ID eScreenId, const CYIString &name, const CYIString &layout, NavigationInterface &navigationInterface, CYIApp &app);
    virtual ~TabbedScreenViewController();
    
    CYISignal<> UnloadScreen;

protected:
    virtual CYISceneView *BuildView();

    virtual void OnInitScreen();
    virtual void OnPreloadValidateScreen(CYIBundle bundle);
    virtual void OnPreloadScreen(CYIBundle bundle, const CYIPersistentStore &stateData);
    virtual void OnLoadScreen(const CYIPersistentStore &stateData);
    virtual void OnUnloadScreen(CYIPersistentStore &stateData);
    virtual void OnStageScreen();
    virtual void OnUnstageScreen();

    TabbedViewController m_tabbedViewController;

private:
    const CYIString m_layout;

    YI_DISALLOW_COPY_AND_ASSIGN(TabbedScreenViewController)
};

#endif // _TABBED_SCREEN_VIEW_CONTROLLER_H_

