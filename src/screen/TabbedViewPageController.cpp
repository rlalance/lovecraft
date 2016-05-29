#include "screen/TabbedViewPageController.h"

#include "utilities/FunctorUtilities.h"

#include <asset/YiAssetViewTemplate.h>
#include <utility/YiError.h>
#include <utility/YiString.h>

static const CYIString TAG = "TabbedViewPageController";

YI_RTTI_DEF(TabbedViewPageController, "TabbedViewPageController")

TabbedViewPageController::TabbedViewPageController()
    : m_pPage(YI_NULL)
    , m_navigationInterface(YI_NULL)
    , m_bIsPreloaded(false)
    , m_bIsLoaded(false)
    , m_bIsSuspended(false)
{
}

TabbedViewPageController::~TabbedViewPageController()
{
    std::for_each(m_bundleAugmentors.begin(), m_bundleAugmentors.end(), Utility::DefaultDelete());
}

CYISceneView *TabbedViewPageController::BuildDefaultView(CYISceneManager *pSceneManager)
{
    CYISceneView *pDefaultView = new CYISceneView();

    pDefaultView->Init();

    return pDefaultView;
}


bool TabbedViewPageController::IsPreloaded() const
{
    return m_bIsPreloaded;
}

bool TabbedViewPageController::IsLoaded() const
{
    return m_bIsLoaded;
}

bool TabbedViewPageController::IsSuspended() const
{
    return m_bIsSuspended;
}

CYISceneView *TabbedViewPageController::BuildViewFromTemplate(const CYISharedPtr<CYIAssetViewTemplate> &pViewTemplate, CYISceneManager *pSceneManager)
{
    YI_ASSERT(pViewTemplate, TAG, "Invalid view template");

    CYIScopedPtr<CYISceneView> pView(pViewTemplate->BuildView(pSceneManager));
    YI_ASSERT(pView, TAG, "Could not instantiate view");

    if (!pView->Init())
    {
        YI_ASSERT(false, TAG, "Could not initialize view");

        return YI_NULL;
    }

    return pView.Take();
}

void TabbedViewPageController::Init(TabbedViewPage *pPage)
{
    YI_ASSERT(pPage, TAG, "Invalid page");

    m_pPage = pPage;

    OnInit();
}

void TabbedViewPageController::Preload(CYIBundle bundle)
{
    if (!m_bIsPreloaded)
    {
        std::vector<BundleAugmentor *>::const_iterator bundleAugmentor = m_bundleAugmentors.begin();

        while (bundleAugmentor != m_bundleAugmentors.end())
        {
            (*bundleAugmentor++)->AugmentBundle(bundle);
        }
        
        OnPreload(bundle);
        m_bIsPreloaded = true;
    }
}

void TabbedViewPageController::Load()
{
    if (m_bIsPreloaded && !m_bIsLoaded)
    {
        OnLoad();
        m_bIsLoaded = true;
    }
}

void TabbedViewPageController::Unload()
{
    if (m_bIsPreloaded && m_bIsLoaded)
    {
        OnUnload();
        m_bIsLoaded = false;
    }
}

void TabbedViewPageController::Resume()
{
    if (m_bIsPreloaded && m_bIsLoaded && m_bIsSuspended)
    {
        OnResume();
        m_bIsSuspended = false;
    }
}

void TabbedViewPageController::Suspend()
{
    if (m_bIsPreloaded && m_bIsLoaded && !m_bIsSuspended)
    {
        OnSuspend();
        m_bIsSuspended = true;
    }
}

void TabbedViewPageController::OnInit()
{
}

void TabbedViewPageController::OnPreload(CYIBundle)
{
}

void TabbedViewPageController::OnLoad()
{
}

void TabbedViewPageController::OnUnload()
{
}

void TabbedViewPageController::OnResume()
{
}

void TabbedViewPageController::OnSuspend()
{
}
