#ifndef _LOVECRAFT_APP_H_
#define _LOVECRAFT_APP_H_

#include "app/ApplicationConfiguration.h"
#include "app/AppController.h"

#include <framework/YiApp.h>
#include <smartptr/YiScopedPtr.h>

class LovecraftApp : public CYIApp
{
public:
    LovecraftApp();
    virtual ~LovecraftApp();

    static void SetupApplicationConfiguration();
    static void SetApplicationFormFactor(ApplicationConfiguration& applicationConfiguration);
    static void SetApplicationPlatform(ApplicationConfiguration& applicationConfiguration);
    /*!
    \details Application set up and initialization occurs here. This sample minimally loads and displays an exported layout.
    */
    virtual bool UserInit();

    /*!
     \details Called when both engine and user initialization has completed. UserStart() will be the first thing called when the application is ready to begin. 
     */
    virtual bool UserStart();

    /*!
     \details Called on each frame before the update of the scene tree. This function may be used to drive other time-dependent components of an application which are not managed by You.i Engine. 
     */
    virtual void UserUpdate();
    void Tests() const;

private:
    CYIScopedPtr<AppController> m_pAppController;

};

#endif
