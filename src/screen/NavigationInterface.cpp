#include "screen/NavigationInterface.h"

const CYIString NavigationInterface::ASSET_PACKAGE_BUNDLE_KEY = "assetPackage";
const CYIString NavigationInterface::PAGE_BUNDLE_KEY = "page";

void NavigationInterface::NavigateToScreen(SCREEN_ID eScreenId)
{
    NavigateToScreen(eScreenId, CYIBundle());
}

void NavigationInterface::NavigateToScreen(SCREEN_ID eScreenId, const CYIBundle &rBundle)
{
    NavigateToScreen(eScreenId, rBundle);
}
