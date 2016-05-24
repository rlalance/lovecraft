#ifndef _SCREEN_VIEW_CONTROLLER_H_
#define _SCREEN_VIEW_CONTROLLER_H_

#include "screen/NavigationInterface.h"

#include <screen/YiAbstractScreenViewController.h>

class ScreenViewController : public CYIAbstractScreenViewController
{

public:
    static ScreenViewController *Create(NavigationInterface::SCREEN_ID eScreenId, NavigationInterface &rNavigationInterface, CYIApp &rApp);
    virtual ~ScreenViewController();

    NavigationInterface::SCREEN_ID GetScreenId() const;
    const CYIString &GetScreenName() const;

protected:
    ScreenViewController(NavigationInterface::SCREEN_ID eScreenId, const CYIString &rName, NavigationInterface &rNavigationInterface, CYIApp &rApp);

    NavigationInterface &m_navigationInterface;

private:
    const NavigationInterface::SCREEN_ID m_eScreenId;
    const CYIString m_name;

    YI_DISALLOW_COPY_AND_ASSIGN(ScreenViewController);
};

#endif

