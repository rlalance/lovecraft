#include "screen/NavigationController.h"

//#include "bridges/PlatformEventBridge.h"
//#include "models/UserDataModel.h"
#include "screen/ScreenViewController.h"
#include "utilities/FunctorUtilities.h"

#include <framework/YiApp.h>
#include <platform/YiApplicationNavigationBarBridge.h>
#include <platform/YiApplicationOrientationBridge.h>
#include <platform/YiApplicationStatusBarBridge.h>
#include <platform/YiApplicationUIBridgeLocator.h>
#include <screen/YiScreenTransition.h>

static const CYIString TAG("NavigationController");

static inline CYIScreenTransitionManager::ScreenTransitionBundle TransitionBundleFor(ScreenViewController *pScreenViewController, const CYIBundle &rBundle = CYIBundle())
{
    return CYIScreenTransitionManager::ScreenTransitionBundle(pScreenViewController, rBundle);
}

NavigationController::NavigationController(CYIApp &rApp)
    : m_rApp(rApp)
    , m_ActivityIndicator(m_rApp.GetSceneManager(), "ActivityIndicator", "Activity_Main.layout", 0)
    , m_ScreenTransition(&m_ActivityIndicator, 0)
    , m_bIsStarted(false)
{
    m_ActivityIndicator.Init();
    m_ScreenTransitionManager.UseTransition(&m_ScreenTransition);
    m_ScreenTransitionManager.SetTransitionDelegate(this);
    
    CYIBackButtonHandler::AddListener(this);
}

NavigationController::~NavigationController()
{
    std::for_each(m_ScreenViewControllers.begin(), m_ScreenViewControllers.end(), Utility::DefaultDelete());
    
    CYIBackButtonHandler::RemoveListener(this);
}

void NavigationController::Start(SCREEN_ID eScreenId)
{
    m_bIsStarted = true;

    if (m_NavigationRequests.size() > 0)
    {
        NavigationRequest navigationRequest = m_NavigationRequests.back();
        NavigateToScreen(navigationRequest.eScreenId, navigationRequest.bundle);

        // Empty the queue. We only care about the last item if more than
        // one request has come in before the controller is started.
        while (!m_NavigationRequests.empty()) 
        {
            m_NavigationRequests.pop();
        }
    }
    else
    {
        NavigationInterface::NavigateToScreen(eScreenId);
    }
}

void NavigationController::NavigateToScreen(SCREEN_ID eScreenId, const CYIBundle &rBundle)
{
    // If there is an active transition we don't want to create and push a new screenviewcontroller to the
    // controllers list and connect it to the screentransitionmanager, because CYIScreenTransitionManager::PushScreen()
    // runs this check internally and can silently fail and then on our end unused/tracked screenviewcontroller's can get created
    // and  connected to the transition manager but not pushed to it causing navigation crashes on Android devices, possibly even leaks, etc.
    if (!m_ScreenTransitionManager.IsTransitionActive())
    {
        if (!m_bIsStarted)
        {
            m_NavigationRequests.push(NavigationRequest(eScreenId, rBundle));
        }
        else if (ScreenViewController *pScreenViewController = CreateScreenViewController(eScreenId))
        {
            m_ScreenTransitionManager.PushScreen(pScreenViewController, rBundle);
        }
    }
}

std::vector<CYIScreenTransitionManager::ScreenTransitionBundle> NavigationController::BuildScreenHistoryFor(ScreenViewController *pScreenViewController)
{
    std::vector<CYIScreenTransitionManager::ScreenTransitionBundle> screenHistory;

    switch (pScreenViewController->GetScreenId())
    {
    case SPLASH:
        screenHistory.push_back(TransitionBundleFor(CreateScreenViewController(SPLASH)));
        break;
    case BOOK:
        screenHistory.push_back(TransitionBundleFor(CreateScreenViewController(BOOK)));
        break;
    }

    return screenHistory;
}

bool NavigationController::IsScreenAvailable() const
{
    return m_ScreenTransitionManager.PeekScreen() != YI_NULL;
}

void NavigationController::NavigateBack()
{
    OnBackButtonPressed();
}

bool NavigationController::OnBackButtonPressed()
{
    // Ignore backbutton request if NavigationController is not started yet.
    if (m_bIsStarted && !m_ScreenTransitionManager.PopScreen())
    {
        return false;
    }
    return true;
}

ScreenViewController *NavigationController::CreateScreenViewController(SCREEN_ID eScreenId)
{
    ScreenViewController *pScreenViewController(ScreenViewController::Create(eScreenId, *this, m_rApp));

    if (pScreenViewController)
    {
        if (std::find(m_ScreenViewControllers.begin(), m_ScreenViewControllers.end(), pScreenViewController) == m_ScreenViewControllers.end())
        {
            m_ScreenTransitionManager.Connect(pScreenViewController);
            m_ScreenViewControllers.push_back(pScreenViewController);
        }
        else
        {
            YI_ASSERT(false, TAG, "Screen view controller is not unique");

            delete pScreenViewController;
        }
    }
    else
    {
        YI_ASSERT(false, TAG, "Screen view controller was not created");
    }

    return pScreenViewController;
}

void NavigationController::onSurfaceChangeRequested(CYIScreenTransitionManager *, YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility)
{
    UpdateApplicationSurface(orientation, statusBarVisibility, navigationBarVisibility);
}

void NavigationController::onSurfaceReady(CYIScreenTransitionManager *, YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility)
{
    //TODO MAYBE?
    //PlatformEventBridge::GetInstance().PrepareRenderingSurface();

    //UpdateApplicationSurface(orientation, statusBarVisibility, navigationBarVisibility);
}

void NavigationController::OnPopScreen(CYIScreenTransitionManager *, CYIAbstractScreenViewController *pScreenViewController)
{
    m_ScreenViewControllers.remove(static_cast<ScreenViewController *>(pScreenViewController));

    delete pScreenViewController;
}

void NavigationController::UpdateApplicationSurface(YIViewProperty::Orientation orientation, YIViewProperty::StatusBar statusBarVisibility, YIViewProperty::NavigationBar navigationBarVisibility)
{
    // Force the platform orientation to our orientation
    CYIApplicationOrientationBridge *pOrientationBridge = CYIApplicationUIBridgeLocator::GetApplicationOrientationBridge();
    if (pOrientationBridge)
    {
        pOrientationBridge->SetOrientation(orientation);
    }
    
    // Force the status bar visibility to our visibility
    CYIApplicationStatusBarBridge *pStatusBarBridge = CYIApplicationUIBridgeLocator::GetApplicationStatusBarBridge();
    if (pStatusBarBridge)
    {
        pStatusBarBridge->SetStatusBarVisibility(statusBarVisibility);
    }
    
    CYIApplicationNavigationBarBridge *pNavigationBarBridge = CYIApplicationUIBridgeLocator::GetApplicationNavigationBarBridge();
    if (pNavigationBarBridge)
    {
        pNavigationBarBridge->SetNavigationBarVisibility(navigationBarVisibility);
    }
}
