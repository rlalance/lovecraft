#ifndef DEVICE_UTILITY_BRIDGE_H
#define DEVICE_UTILITY_BRIDGE_H

#include <framework/YiPredef.h>

class DeviceUtilityBridge
{
public:

    enum DEVICE_FORM_FACTOR
    {
        DEVICE_FORM_FACTOR_UNKNOWN,
        DEVICE_FORM_FACTOR_HANDSET,
        DEVICE_FORM_FACTOR_TABLET,
        DEVICE_FORM_FACTOR_TV
    };

    /*!
     \details Return the form factor of the current device.  Determination of the form factor is platform specific.
     */
    
    static DEVICE_FORM_FACTOR GetDeviceFormFactor();
};

#endif
