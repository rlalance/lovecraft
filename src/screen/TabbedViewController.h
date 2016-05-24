#ifndef _TABBED_VIEW_CONTROLLER_H_
#define _TABBED_VIEW_CONTROLLER_H_

#include <signal/YiSignal.h>
#include <signal/YiSignalHandler.h>
#include <utility/YiBundle.h>

class NavigationInterface;
class TabbedView;
class TabbedViewPage;
class TabbedViewPageController;

class CYIBundle;

/*!
    \details TabbedViewController is a view controller for TabbedView. It is not
    intended to be subclassed. Instances of TabbedViewController are responsible
    for creating and managing TabbedViewPageControllers for any TabbedViewPages
    in the TabbedView they are associated with that have view controllers defined.
*/
class TabbedViewController : public CYISignalHandler
{
    typedef std::map<TabbedViewPage *, TabbedViewPageController *> PageControllers;

public:

    TabbedViewController(NavigationInterface &navigationInterface);

    virtual ~TabbedViewController();

    /*!
        \details Returns the  tabbed view.
    */
    TabbedView *GetTabbedView() const;

    /*!
        \details Initializes itself around the given tabbed view.
    */
    virtual void Init(TabbedView *pTabbedView);

    /*!
        \details Should be called by a screen view controller when it's preloading.
    */
    virtual void Preload(CYIBundle bundle);

    /*!
        \details Should be called by a screen view controller when it's loading.
    */
    virtual void Load();
    
    /*!
        \details Should be called by a screen view controller when it's unloading.
    */
    virtual void Unload();

    /*!
        \details Constructs a controller with the given name and uses it to
        build a default view which is then added to the tabbed view.
    */
    TabbedViewPageController *AddPageUsingNamedController(const CYIString &controllerName);

    /*!
        \details Constructs a controller with the given name and uses it to build a
        default view which is then inserted to the tabbed view at the given index.
    */
    TabbedViewPageController *InsertPageUsingNamedController(YI_INT32 nPageIndex, const CYIString &controllerName);

    /*!
        \details Returns the view controller for the given page.
    */
    TabbedViewPageController *GetControllerForPage(TabbedViewPage *pPage) const;

    /*!
        \details Returns the view controller for the page at the given index.
    */
    TabbedViewPageController *GetViewControllerForPageAtIndex(YI_INT32 nPageIndex) const;

    /*!
        \details Returns the view controller for the page with the given name.
    */
    template<typename ControllerType>
    ControllerType *GetViewControllerForPageAtIndex(YI_INT32 nPageIndex) const;

    /*!
        \details Returns the view controller for the page at the given index.
    */
    TabbedViewPageController *GetViewControllerForPageWithName(const CYIString &pageName) const;

    /*!
        \details Returns the view controller for the page with the given name.
    */
    template<typename ControllerType>
    ControllerType *GetViewControllerForPageWithName(const CYIString &pageName) const;

protected:

    /*!
        \details Instantiates the view controller with the given name.
    */
    static TabbedViewPageController *CreateNamedController(const CYIString &controllerName);

    /*!
        \details Configures the given controller for the given page.
    */
    void ConfigureControllerForPage(TabbedViewPageController *pController, TabbedViewPage *pPage);

    /*!
        \details Resumes the given page.
    */
    virtual void ResumeControllerForPage(TabbedViewPage *pPage);

    /*!
        \details Suspends the given page.
    */
    virtual void SuspendControllerForPage(TabbedViewPage *pPage);

    NavigationInterface &m_navigationInterface;

    TabbedView *m_pTabbedView;

    PageControllers m_pageControllers;

private:

    YI_DISALLOW_COPY_AND_ASSIGN(TabbedViewController);
};

#include "screen/TabbedViewController_inl.h"

#endif // _TABBED_VIEW_CONTROLLER_H_

