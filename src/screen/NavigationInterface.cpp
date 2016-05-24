#include "screen/NavigationInterface.h"

const CYIString NavigationInterface::ASSET_PACKAGE_BUNDLE_KEY = "assetPackage";
const CYIString NavigationInterface::AUTH_LEVEL_ID_BUNDLE_KEY = "authLevel";
const CYIString NavigationInterface::BRANDING_CHANNEL_ID_BUNDLE_KEY = "brandingChannel";
const CYIString NavigationInterface::SCREEN_CONTAINER_BUNDLE_KEY = "screenContainer";
const CYIString NavigationInterface::PAGE_CONTAINER_BUNDLE_KEY = "pageContainer";
const CYIString NavigationInterface::PAGE_BUNDLE_KEY = "page";
const CYIString NavigationInterface::RESOURCE_TYPE_BUNDLE_KEY = "resourceType";

void NavigationInterface::NavigateToScreen(SCREEN_ID eScreenId)
{
    NavigateToScreen(eScreenId, NO_REASON, CYIBundle());
}

void NavigationInterface::NavigateToScreen(SCREEN_ID eScreenId, REASON eReason)
{
    NavigateToScreen(eScreenId, eReason, CYIBundle());
}

void NavigationInterface::NavigateToScreen(SCREEN_ID eScreenId, const CYIBundle &rBundle)
{
    NavigateToScreen(eScreenId, NO_REASON, rBundle);
}
