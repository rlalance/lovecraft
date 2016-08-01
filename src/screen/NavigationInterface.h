#ifndef _NAVIGATION_INTERFACE_H_
#define _NAVIGATION_INTERFACE_H_

#include <signal/YiSignalHandler.h>
#include <utility/YiBundle.h>

class NavigationInterface : public CYISignalHandler
{

public:
    enum SCREEN_ID
    {
        SPLASH,
        BOOK
    };

    static const CYIString ASSET_PACKAGE_BUNDLE_KEY;
    static const CYIString PAGE_BUNDLE_KEY;

    NavigationInterface() {}
    virtual ~NavigationInterface() {}

    void NavigateToScreen(SCREEN_ID eScreenId);
    virtual void NavigateToScreen(SCREEN_ID eScreenId, const CYIBundle &rBundle);

    virtual void NavigateBack() = 0;

};

#endif // _NAVIGATION_INTERFACE_H_

