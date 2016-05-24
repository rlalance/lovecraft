#include "screen/TabbedViewController.h"

#include "customviews/TabbedView.h"
#include "customviews/TabbedViewPage.h"
#include "screen/TabbedViewPageController.h"
#include "utilities/FunctorUtilities.h"

#include <asset/YiAssetViewTemplate.h>
#include <import/YiViewTemplate.h>

static const CYIString TAG = "TabbedViewController";

TabbedViewController::TabbedViewController(NavigationInterface &navigationInterface)
    : m_navigationInterface(navigationInterface)
    , m_pTabbedView(YI_NULL)
{
}

TabbedViewController::~TabbedViewController()
{
    std::for_each(m_pageControllers.begin(), m_pageControllers.end(), Utility::DefaultDelete());
}

TabbedView *TabbedViewController::GetTabbedView() const
{
    return m_pTabbedView;
}

void TabbedViewController::Init(TabbedView *pTabbedView)
{
    YI_ASSERT(pTabbedView, TAG, "Invalid tabbed view");

    m_pTabbedView = pTabbedView;

    for (YI_INT32 nPageIndex = 0; nPageIndex < m_pTabbedView->GetPageCount(); ++nPageIndex)
    {
        TabbedViewPage *pPage = m_pTabbedView->GetPageAtIndex(nPageIndex);

        const CYIString controllerName = pPage->GetControllerName();

        if (controllerName.IsNotEmpty())
        {
            ConfigureControllerForPage(CreateNamedController(controllerName), pPage);
        }
    }
}

void TabbedViewController::Preload(CYIBundle bundle)
{
    for (PageControllers::const_iterator iter = m_pageControllers.begin(); iter != m_pageControllers.end(); ++iter)
    {
        iter->second->Preload(bundle);
    }
}

void TabbedViewController::Load()
{
    TabbedViewPageController *pController = GetControllerForPage(m_pTabbedView->GetCurrentPage());

    if (pController != YI_NULL)
    {
        pController->Load();
    }
}

void TabbedViewController::Unload()
{
    for (PageControllers::const_iterator iter = m_pageControllers.begin(); iter != m_pageControllers.end(); ++iter)
    {
        iter->second->Unload();
    }
}

void TabbedViewController::ResumeControllerForPage(TabbedViewPage *pPage)
{
    TabbedViewPageController *pController = GetControllerForPage(pPage);

    if (pController != YI_NULL)
    {
        if (!pController->IsLoaded())
        {
            pController->Load();
        }
        else
        {
            pController->Resume();
        }
    }
}

void TabbedViewController::SuspendControllerForPage(TabbedViewPage *pPage)
{
    TabbedViewPageController *pController = GetControllerForPage(pPage);

    if (pController != YI_NULL)
    {
        pController->Suspend();
    }
}

TabbedViewPageController *TabbedViewController::AddPageUsingNamedController(const CYIString &controllerName)
{
    return InsertPageUsingNamedController(m_pTabbedView->GetPageCount(), controllerName);
}

TabbedViewPageController *TabbedViewController::InsertPageUsingNamedController(YI_INT32 nPageIndex, const CYIString &controllerName)
{
    YI_ASSERT(controllerName.IsNotEmpty(), TAG, "Invalid controller name");
    YI_ASSERT(nPageIndex >= 0 && nPageIndex <= m_pTabbedView->GetPageCount(), TAG, "Invalid page index");

    TabbedViewPageController *pController = CreateNamedController(controllerName);
    CYISceneView *pView = pController->BuildDefaultView(m_pTabbedView->GetSceneManager());

    TabbedViewPage *pTabbedViewPage = m_pTabbedView->InsertPage(nPageIndex, pView);
    pTabbedViewPage->SetProperty("name", controllerName);
    ConfigureControllerForPage(pController, pTabbedViewPage);

    return pController;
}

TabbedViewPageController *TabbedViewController::CreateNamedController(const CYIString &controllerName)
{
    TabbedViewPageController *pController = YiRTTINew<TabbedViewPageController>(controllerName.GetData());

    YI_ASSERT(pController, TAG, "Could not instantiate controller '%s'", controllerName.GetData());

    return pController;
}

void TabbedViewController::ConfigureControllerForPage(TabbedViewPageController *pController, TabbedViewPage *pPage)
{
    pController->Init(pPage);
    pController->m_navigationInterface = &m_navigationInterface;
    pPage->PageShown.Connect(*this, &TabbedViewController::ResumeControllerForPage);
    pPage->PageHidden.Connect(*this, &TabbedViewController::SuspendControllerForPage);
    m_pageControllers[pPage] = pController;
}

TabbedViewPageController *TabbedViewController::GetControllerForPage(TabbedViewPage *pPage) const
{
    const PageControllers::const_iterator iter = m_pageControllers.find(pPage);

    if (iter != m_pageControllers.end())
    {
        return iter->second;
    }
    return YI_NULL;
}

TabbedViewPageController *TabbedViewController::GetViewControllerForPageAtIndex(YI_INT32 nPageIndex) const
{
    return GetControllerForPage(m_pTabbedView->GetPageAtIndex(nPageIndex));
}

TabbedViewPageController *TabbedViewController::GetViewControllerForPageWithName(const CYIString &pageName) const
{
    return GetControllerForPage(m_pTabbedView->GetPageWithName(pageName));
}
