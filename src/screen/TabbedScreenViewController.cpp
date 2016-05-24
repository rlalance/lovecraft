#include "screen/TabbedScreenViewController.h"

#include "customviews/ButtonBarView.h"
#include "customviews/TabbedView.h"
#include "utilities/SceneLayers.h"
#include "utilities/NodeUtilities.h"

static const CYIString TAG = "TabbedScreenViewController";

TabbedScreenViewController::TabbedScreenViewController(NavigationInterface::SCREEN_ID eScreenId, const CYIString &name, const CYIString &layout, NavigationInterface &navigationInterface, CYIApp &app)
    : ScreenViewController(eScreenId, name, navigationInterface, app)
    , m_tabbedViewController(navigationInterface)
    , m_layout(layout)
{
}

TabbedScreenViewController::~TabbedScreenViewController()
{
}

CYISceneView *TabbedScreenViewController::BuildView()
{
    YI_ASSERT(m_layout.IsNotEmpty(), TAG, "Layout undefined");

    CYISceneView *pSceneView = m_pSceneManager->LoadScene(m_layout, CYISceneManager::SCALE_RESPONSIVE_LAYOUT, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);
    YI_ASSERT(pSceneView, TAG, "Scene manager failed to load '%s'", m_layout.GetData());

    m_pSceneManager->AddScene(GetScreenName(), pSceneView, SceneLayers::LAYER_SCREENS);
    m_pSceneManager->UnstageScene(GetScreenName());

    return pSceneView;
}

void TabbedScreenViewController::OnInitScreen()
{
    TabbedView *pTabbedView = Utility::GetNode<TabbedView>(m_pView, "TabbedView");
    ButtonBarView *pButtonBarView = Utility::GetNode<ButtonBarView>(m_pView, "ButtonBar", Utility::GET_NODE_FLAG_OPTIONAL);

    m_tabbedViewController.Init(pTabbedView);

    if (pButtonBarView != YI_NULL)
    {
        pButtonBarView->SelectButton(pTabbedView->GetCurrentPageIndex());
        pButtonBarView->ButtonSelected.Connect(*pTabbedView, &TabbedView::ShowPageAtIndex);
        pTabbedView->PageShownAtIndex.Connect(*pButtonBarView, &ButtonBarView::SelectButton);
    }
}

void TabbedScreenViewController::OnPreloadValidateScreen(CYIBundle)
{
}
                             
void TabbedScreenViewController::OnPreloadScreen(CYIBundle bundle, const CYIPersistentStore &)
{
    m_tabbedViewController.Preload(bundle);

    CYIString pageName;

    if (bundle.Get(NavigationInterface::PAGE_BUNDLE_KEY, &pageName) && pageName.IsNotEmpty())
    {
        m_tabbedViewController.GetTabbedView()->ShowPageWithName(pageName, false);
    }
}

void TabbedScreenViewController::OnLoadScreen(const CYIPersistentStore &)
{
    m_tabbedViewController.Load();
}

void TabbedScreenViewController::OnUnloadScreen(CYIPersistentStore &)
{
    m_tabbedViewController.Unload();

    UnloadScreen();
}

void TabbedScreenViewController::OnStageScreen()
{
    m_pSceneManager->StageScene(GetScreenName());
}

void TabbedScreenViewController::OnUnstageScreen()
{
    m_pSceneManager->UnstageScene(GetScreenName());
}
