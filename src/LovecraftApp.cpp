#include "LovecraftApp.h"

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

static const CYIString TAG = "LovecraftApp";

LovecraftApp::LovecraftApp()
{
}

LovecraftApp::~LovecraftApp()
{
}

bool LovecraftApp::UserInit()
{
    SetupApplicationConfiguration();

    m_pAppController = new AppController(*this);

    // Load a layout file which will be the root scene view.
    CYISceneView *pSceneViewMain = GetSceneManager()->LoadScene("MainNav_Main.layout", CYISceneManager::SCALE_STRETCH, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);

    if(!pSceneViewMain)
    {
        YI_LOGE("LovecraftApp::UserInit", "Loading scene has failed");
        return false;
    }

    pSceneViewMain->GetInTimeline()->StartForward();

    // Add the scene view to the scene manager.
    GetSceneManager()->AddScene("MainComp", pSceneViewMain, 0, CYISceneManager::LAYER_OPAQUE);

    return true;
}

void LovecraftApp::SetupApplicationConfiguration() const
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

void LovecraftApp::SetApplicationFormFactor(ApplicationConfiguration &applicationConfiguration) const
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

void LovecraftApp::SetApplicationPlatform(ApplicationConfiguration &applicationConfiguration) const
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

bool LovecraftApp::UserStart()
{
    CYIString assetPath = CYIApp::GetAssetsPath();
    QuestList* quests = QuestList::FromJSON(assetPath + "resources/Quests.json");

    YI_LOGI("LovecraftApp::UserStart", "%s", quests->ToString().GetData());

    delete quests;

    return m_pAppController->Start();
}

void LovecraftApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}
