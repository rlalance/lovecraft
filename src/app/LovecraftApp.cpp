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

    //TestLoadingTMX();

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
    CYIString assetPath = GetAssetsPath();
    CYIString questsPath = assetPath + "resources/" + "Quests.json";

    QuestManager* questManager = new QuestManager(questsPath);

    TestQuestProgression(questManager);

    delete questManager;
}

void LovecraftApp::TestLoadingTMX()
{
    //TMX loading test
    tmxparser::TmxReturn error;
    tmxparser::TmxMap map;

    // test from file
    CYIString assetPath = GetAssetsPath();

    CYIString levelPath = assetPath + "resources/" + "test_xml_level.tmx";

    error = tmxparser::parseFromFile(levelPath.GetData(), &map, assetPath.GetData());

    CYIAssetLoader *pAssetLoader = CYIFramework::GetInstance()->GetAssetLoader();

    CYISharedPtr<CYIAsset> pAsset = pAssetLoader->Load(YiGetTypeId<AssetTMX>(), "drawable/default/test_xml_level.tmx", YI_NULL);
}

void LovecraftApp::TestQuestProgression(QuestManager* quest_manager)
{
    YI_LOGI("LovecraftApp::TestQuestProgression", "ToString: \n%s", quest_manager->AllQuestsToString().GetData());

    YI_LOGI("LovecraftApp::TestQuestProgression", "DisplayText: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After Speaking to handler
    quest_manager->ActivateCondition(CYIString("speak:handler:briefing"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After talking to handler: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After picking up healthkit and taser
    quest_manager->ActivateCondition(CYIString("pickup:item:healthkit"));
    quest_manager->ActivateCondition(CYIString("pickup:weapon:tazer"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After picking up healthkit and taser: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After finding hideout
    quest_manager->ActivateCondition(CYIString("discover:location:prison"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After finding hideout: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After securing the hideout
    quest_manager->ActivateCondition(CYIString("use:interactable:prison_computer"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After securing the hideout: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());
    
    //Saving the prince
    quest_manager->ActivateCondition(CYIString("rescue:maincharacter:prince"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After saving the prince: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After killing the prince
    quest_manager->ActivateCondition(CYIString("state:prince:dead"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After killing the prince: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After saving the advisor
    quest_manager->ActivateCondition(CYIString("rescue:maincharacter:chiefadvisor"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After saving the advisor: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After saving the advisor
    quest_manager->ActivateCondition(CYIString("rescue:maincharacter:chiefadvisor"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After saving the advisor: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After the scientist transcends
    quest_manager->ActivateCondition(CYIString("state:head_scientist:transcended"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After the scientist transcends: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

    //After saving the Fellow Agent
    quest_manager->ActivateCondition(CYIString("rescue:maincharacter:fellowagent"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After saving the Fellow Agent: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());
    
    //After speaking to the Advisor about the conflict
    quest_manager->ActivateCondition(CYIString("speak:chief_advisor:conflict"));

    YI_LOGI("LovecraftApp::TestQuestProgression", "After speaking to the Advisor about the conflict: \n%s", quest_manager->GetAllQuestsDisplayText().GetData());

}