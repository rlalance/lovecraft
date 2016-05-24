#ifndef _NAVIGATION_INTERFACE_H_
#define _NAVIGATION_INTERFACE_H_

#include <signal/YiSignalHandler.h>
#include <utility/YiBundle.h>

class NavigationInterface : public CYISignalHandler
{

public:
    enum SCREEN_ID
    {
        COLLECTION,
        CONTENT,
        LANDER,
        LOGIN,
        PLAYER
    };

    enum REASON
    {
        NO_REASON,
        DEEPLINK_REASON
    };

    static const CYIString ASSET_PACKAGE_BUNDLE_KEY;
	static const CYIString AUTH_LEVEL_ID_BUNDLE_KEY;
	static const CYIString BRANDING_CHANNEL_ID_BUNDLE_KEY;
    static const CYIString PAGE_CONTAINER_BUNDLE_KEY;
    static const CYIString PAGE_BUNDLE_KEY;
    static const CYIString RESOURCE_TYPE_BUNDLE_KEY;
	static const CYIString SCREEN_CONTAINER_BUNDLE_KEY;

    NavigationInterface() {}
    virtual ~NavigationInterface() {}

    void NavigateToScreen(SCREEN_ID eScreenId);
    void NavigateToScreen(SCREEN_ID eScreenId, REASON eReason);
    void NavigateToScreen(SCREEN_ID eScreenId, const CYIBundle &rBundle);

    virtual void NavigateToScreen(SCREEN_ID eScreenId, REASON eReason, const CYIBundle &rBundle) = 0;
    virtual void NavigateBack() = 0;

};

#endif // _NAVIGATION_INTERFACE_H_

