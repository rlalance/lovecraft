#ifndef _TABBED_VIEW_PAGE_CONTROLLER_H_
#define _TABBED_VIEW_PAGE_CONTROLLER_H_

#include "customviews/TabbedViewPage.h"

#include <framework/YiAny.h>
#include <framework/YiPredef.h>
#include <utility/YiBundle.h>
#include <utility/YiRtti.h>

class NavigationInterface;
class TabbedViewController;

class CYIBundle;
class CYISceneManager;

/*!
    \details TabbedViewPageController is a view controller for TabbedViewPage.
    TabbedViewPageController is not intended to be used directly but rather via
    a subclass. TabbedViewPageControllers are created, owned and put into various 
    states by TabbedViewController. The diagram below illustrates these states.
 
                               +-------+
                         +---->|Suspend|-------+
                         |     +-------+       |
                         |       |   ^         v
          +-------+   +----+     |   |     +------+
     O--->|Preload|-->|Load|-------------->|Unload|---+---X
          +-------+   +----+     |   |     +------+   |
              ^                  v   |         ^      |
              |                +-------+       |      |
              |                |Resume |-------+      |
              |                +-------+              |
              |                                       |
              +---------------------------------------+

*/
class TabbedViewPageController
{
    friend class TabbedViewController;

public:

    TabbedViewPageController();
    virtual ~TabbedViewPageController();

    /*!
        \details Returns true if the page controller is preloaded.
    */
    bool IsPreloaded() const;

    /*!
        \details Returns true if the page controller is loaded.
    */
    bool IsLoaded() const;

    /*!
        \details Returns true if the page controller is suspended.
    */
    bool IsSuspended() const;

    /*!
        \details Augments the bundle passed to the page controller on preload with the given data.
    */
    template<typename ValueType>
    void AugmentBundleOnPreloadWithData(const CYIString &key, const ValueType &value);

protected:

    /*!
        \details Builds a view from the given template in the given scene manager.
    */
    static CYISceneView *BuildViewFromTemplate(const CYISharedPtr<CYIAssetViewTemplate> &pViewTemplate, CYISceneManager *pSceneManager);

    /*!
        \details Reimplement to build a default view for the controller.
    */
    virtual CYISceneView *BuildDefaultView(CYISceneManager *pSceneManager);

    /*!
        \details Reimplement to handle initialization.
    */
    virtual void OnInit();

    /*!
        \details Reimplement to handle page preload.
    */
    virtual void OnPreload(CYIBundle pageBundle);

    /*!
        \details Reimplement to handle page load.
    */
    virtual void OnLoad();

    /*!
        \details Reimplement to handle page unload.
    */
    virtual void OnUnload();

    /*!
        \details Reimplement to handle page resume.
    */
    virtual void OnResume();

    /*!
        \details Reimplement to handle page suspend.
    */
    virtual void OnSuspend();

    TabbedViewPage *m_pPage;

    NavigationInterface *m_navigationInterface;

private:

    void Init(TabbedViewPage *pPage);
    void Preload(CYIBundle bundle);
    void Load();
    void Unload();
    void Resume();
    void Suspend();

    bool m_bIsPreloaded;
    bool m_bIsLoaded;
    bool m_bIsSuspended;

    struct BundleAugmentor
    {
        virtual ~BundleAugmentor() {}
        virtual void AugmentBundle(CYIBundle &bundle) const = 0;
    };

    std::vector<BundleAugmentor *> m_bundleAugmentors;

    YI_DISALLOW_COPY_AND_ASSIGN(TabbedViewPageController)

    YI_TYPE_DATA
};

#include "screen/TabbedViewPageController_inl.h"

#endif // _TABBED_VIEW_PAGE_CONTROLLER_H_

