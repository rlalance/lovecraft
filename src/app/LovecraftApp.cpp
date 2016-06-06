#include "LovecraftApp.h"

#include "app/ApplicationConfiguration.h"
#include "decoder/TMXDecoder.h"
#include "quest/QuestList.h"
#include "utilities/tmxparser.h"

//#include <platform/YiApplicationUIBridgeLocator.h>
#include <platform/YiDeviceBridgeLocator.h>
//#include <platform/YiSystemInformationBridgeLocator.h>
#include <asset/YiAssetLoader.h>
#include <framework/YiFramework.h>
#include <quest/QuestManager.h>

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

    AddTMXDecoder();

    TestsQuests();

    TestLoadingTMX();

    return true;
}

void LovecraftApp::AddTMXDecoder()
{
    CYIAssetLoader *pAssetLoader = CYIFramework::GetInstance()->GetAssetLoader();

    pAssetLoader->AddDecoder(new TMXDecoder());
}

void LovecraftApp::SetupApplicationConfiguration()
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

void LovecraftApp::SetApplicationFormFactor(ApplicationConfiguration &applicationConfiguration)
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

void LovecraftApp::SetApplicationPlatform(ApplicationConfiguration &applicationConfiguration)
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
    return m_pAppController->Start();
}

void LovecraftApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}


void LovecraftApp::TestsQuests() const
{
    //TMX loading test
    tmxparser::TmxReturn error;
    tmxparser::TmxMap map;

    // test from file
    CYIString assetPath = GetAssetsPath();

    CYIString questsPath = assetPath + "resources/" + "Quests.json";
    CYIString levelPath = assetPath + "resources/" + "test_xml_level.tmx";

    error = tmxparser::parseFromFile(levelPath.GetData(), &map, assetPath.GetData());

    // Quest test
    QuestManager* questManager = new QuestManager(questsPath);
//    QuestList* quests = QuestList::FromJSON(questsPath);

//    YI_LOGI("LovecraftApp::UserStart", "%s", quests->ToString().GetData());
    YI_LOGI("LovecraftApp::UserStart", "%s", questManager->AllQuestsToString().GetData());

    delete questManager;
}

void LovecraftApp::TestLoadingTMX()
{
    CYIAssetLoader *pAssetLoader = CYIFramework::GetInstance()->GetAssetLoader();

    CYISharedPtr<CYIAsset> pAsset = pAssetLoader->Load(YiGetTypeId<AssetTMX>(), "drawable/default/test_xml_level.tmx", YI_NULL);
}