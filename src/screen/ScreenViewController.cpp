#include "screen/ScreenViewController.h"

//#include "player/PlayerScreenViewController.h"
#include "screen/ScreenViewController.h"
#include "screen/SplashScreenViewController.h"
//#include "screen/ContentScreenViewController.h"
//#include "screen/HomeLanderScreenViewController.h"
//#include "screen/LoginScreenViewController.h"

ScreenViewController::ScreenViewController(NavigationInterface::SCREEN_ID eScreenId, const CYIString &rName, NavigationInterface &navigationController, CYIApp &app)
    : CYIAbstractScreenViewController(app.GetSceneManager(), &app)
    , m_navigationInterface(navigationController)
    , m_eScreenId(eScreenId)
    , m_name(CYIString("%1 (%2)").Arg(rName).Arg(CYIString::FromValue(this)))
    , m_app(app)
{

}

ScreenViewController::~ScreenViewController()
{

}

NavigationInterface::SCREEN_ID ScreenViewController::GetScreenId() const
{
    return m_eScreenId;
}

const CYIString &ScreenViewController::GetScreenName() const
{
    return m_name;
}

ScreenViewController *ScreenViewController::Create(NavigationInterface::SCREEN_ID eScreenId, NavigationInterface &navigationController, CYIApp &app)
{
    switch (eScreenId)
    {
    case NavigationInterface::SPLASH:
        return new SplashScreenViewController(navigationController, app);
    }

    return YI_NULL;
}
