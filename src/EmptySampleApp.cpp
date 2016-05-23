// © You i Labs Inc. 2000-2016. All rights reserved.
#include "EmptySampleApp.h"

#include "app/ApplicationConfiguration.h"
#include "quest/QuestModel.h"

#include <datamodel/YiAbstractDataModel.h>
#include <framework/YiFramework.h>
#include <logging/YiLogger.h>
#include <platform/YiApplicationUIBridgeLocator.h>
#include <platform/YiDeviceBridgeLocator.h>
#include <platform/YiSystemInformationBridgeLocator.h>
#include <scenetree/YiSceneManager.h>
#include <view/YiSceneView.h>
#include "quest/QuestList.h"

/*! \addtogroup empty-sample
  @{
 */
static const CYIString TAG = "EmptySampleApp";

EmptySampleApp::EmptySampleApp()
{
}

EmptySampleApp::~EmptySampleApp()
{
}

bool EmptySampleApp::UserInit()
{
    SetupApplicationConfiguration();

    // Load a layout file which will be the root scene view.
    CYISceneView *pSceneViewMain = GetSceneManager()->LoadScene("MainNav_Main.layout", CYISceneManager::SCALE_STRETCH, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);

    if(!pSceneViewMain)
    {
        YI_LOGE("EmptySampleApp::UserInit", "Loading scene has failed");
        return false;
    }

    pSceneViewMain->GetInTimeline()->StartForward();

    // Add the scene view to the scene manager.
    GetSceneManager()->AddScene("MainComp", pSceneViewMain, 0, CYISceneManager::LAYER_OPAQUE);

    return true;
}

void EmptySampleApp::SetupApplicationConfiguration()
{
    ApplicationConfiguration & applicationConfiguration = ApplicationConfiguration::GetInstance();

    applicationConfiguration.AddConfigurations();

    SetApplicationFormFactor(applicationConfiguration);
    //SetApplicationOrientationFromDeviceFormFactor();
    SetApplicationPlatform(applicationConfiguration);
    //SetApplicationLanguage(applicationConfiguration);
    //SetApplicationDataPath(applicationConfiguration);

    applicationConfiguration.RefreshAssetLoaderAssetRootLocator();
}

void EmptySampleApp::SetApplicationFormFactor(ApplicationConfiguration &applicationConfiguration) const
{
    applicationConfiguration.SetFormFactor(FormFactorConfiguration::FORM_FACTOR_TABLET);

    //DeviceUtilityBridge::DEVICE_FORM_FACTOR deviceFormFactor = DeviceUtilityBridge::GetDeviceFormFactor();

    //switch (deviceFormFactor)
    //{
    //case DeviceUtilityBridge::DEVICE_FORM_FACTOR_HANDSET:
    //    applicationConfiguration.SetFormFactor(FormFactorConfiguration::FORM_FACTOR_HANDSET);
    //    break;
    //case DeviceUtilityBridge::DEVICE_FORM_FACTOR_TABLET:
    //    applicationConfiguration.SetFormFactor(FormFactorConfiguration::FORM_FACTOR_TABLET);
    //    break;
    //case DeviceUtilityBridge::DEVICE_FORM_FACTOR_TV:
    //    applicationConfiguration.SetFormFactor(FormFactorConfiguration::FORM_FACTOR_TV);
    //    break;
    //default:
    //    applicationConfiguration.SetFormFactor(FormFactorConfiguration::FORM_FACTOR_TABLET);
    //    break;
    //}
}

void EmptySampleApp::SetApplicationPlatform(ApplicationConfiguration &applicationConfiguration) const
{
    CYIDeviceInformationBridge *pDeviceInformationBridge = CYIDeviceBridgeLocator::GetDeviceInformationBridge();

    if (pDeviceInformationBridge != YI_NULL)
    {
        CYIString deviceOSName = pDeviceInformationBridge->GetDeviceOSName();

        if (deviceOSName == "android")
        {
            applicationConfiguration.SetPlatform(PlatformConfiguration::PLATFORM_ANDROID);
        }
        else if (deviceOSName == "iOS")
        {
            applicationConfiguration.SetPlatform(PlatformConfiguration::PLATFORM_IOS);
        }
        else if (deviceOSName == "OSX")
        {
            applicationConfiguration.SetPlatform(PlatformConfiguration::PLATFORM_OSX);
        }
        else if (deviceOSName == "Windows")
        {
            applicationConfiguration.SetPlatform(PlatformConfiguration::PLATFORM_WINDOWS);
        }
    }
    else
    {
        YI_LOGI(TAG, "Device information bridge not supported on this device.");
    }
}

bool EmptySampleApp::UserStart()
{
    CYIString assetPath = CYIApp::GetAssetsPath();
    QuestList* quests = QuestList::FromJSON(assetPath + "resources/Quests.json");

    YI_LOGI("EmptySampleApp::UserStart", "%s", quests->Display().GetData());

    delete quests;

	return true;
}

void EmptySampleApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}

/*!
@}
*/

