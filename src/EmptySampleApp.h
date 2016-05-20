// © You i Labs Inc. 2000-2016. All rights reserved.
#ifndef _EMPTYSAMPLEAPP_H_
#define _EMPTYSAMPLEAPP_H_

#include "framework/YiApp.h"
#include <app/ApplicationConfiguration.h>

/*!
 \defgroup empty-sample Empty Sample
 \brief An empty sample.
 \details This sample creates a skeleton CYIApp object which displays a blank screen. It is typically used as a starting point for new samples.
 <H3>Purpose:</H3>
 To demonstrate how to build and link a You.i Engine application.
 <H3>Suggested Learning Activities:</H3>
 Build the application on iOS and Android

 */
/*@{*/

class EmptySampleApp : public CYIApp
{
public:
    EmptySampleApp();
    virtual ~EmptySampleApp();

    void SetupApplicationConfiguration();
    void SetApplicationFormFactor(ApplicationConfiguration& applicationConfiguration) const;
    void SetApplicationPlatform(ApplicationConfiguration& applicationConfiguration) const;
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
};

/* @} */

#endif
