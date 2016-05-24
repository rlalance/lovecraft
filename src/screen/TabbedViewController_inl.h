#ifndef _TABBED_VIEW_CONTROLLER_INL_H_
#define _TABBED_VIEW_CONTROLLER_INL_H_

#include <utility/YiRtti.h>

template<typename ControllerType>
ControllerType *TabbedViewController::GetViewControllerForPageAtIndex(YI_INT32 nPageIndex) const
{
    return YiDynamicCast<ControllerType>(GetViewControllerForPageAtIndex(nPageIndex));
}

template<typename ControllerType>
ControllerType *TabbedViewController::GetViewControllerForPageWithName(const CYIString &pageName) const
{
    return YiDynamicCast<ControllerType>(GetViewControllerForPageWithName(pageName));
}

#endif // _TABBED_VIEW_CONTROLLER_INL_H_

