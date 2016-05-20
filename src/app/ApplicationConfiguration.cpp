#include "app/ApplicationConfiguration.h"

#include <asset/YiAssetLocator.h>
#include <asset/YiAssetLoader.h>

YI_CHAR const *const ApplicationConfiguration::CONFIGURATION_NONE = "";

YI_CHAR const *const LanguageConfiguration::LANGUAGE_SPANISH_ARGENTINA  = "es_AR";
YI_CHAR const *const LanguageConfiguration::LANGUAGE_PORTUGUESE_BRAZIL  = "pt_BR";
YI_CHAR const *const LanguageConfiguration::LANGUAGE_ENGLISH_CANADA     = "en_CA";

YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_1x1     = "1x1";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_5x4     = "5x4";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_4x3     = "4x3";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_3x2     = "3x2";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_8x5     = "8x5";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_5x3     = "5x3";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_16x9    = "16x9";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_17x9    = "17x9";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_21x9    = "21x9";
YI_CHAR const *const AspectRatioConfiguration::SCREEN_ASPECT_RATIO_256x135 = "256x135";

YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_HANDSET   = "handset";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_TABLET    = "tablet";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_CAR       = "car";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_DESKTOP   = "desktop";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_TV        = "tv";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_APPLIANCE = "appliance";
YI_CHAR const *const FormFactorConfiguration::FORM_FACTOR_WATCH     = "watch";

YI_CHAR const *const PlatformConfiguration::PLATFORM_ANDROID       = "android";
YI_CHAR const *const PlatformConfiguration::PLATFORM_ROKU          = "roku";
YI_CHAR const *const PlatformConfiguration::PLATFORM_IOS           = "ios";
YI_CHAR const *const PlatformConfiguration::PLATFORM_OSX           = "osx";
YI_CHAR const *const PlatformConfiguration::PLATFORM_APPLETV       = "appletv";
YI_CHAR const *const PlatformConfiguration::PLATFORM_WINDOWS       = "windows";
YI_CHAR const *const PlatformConfiguration::PLATFORM_WINDOWSMOBILE = "windowsmobile";
YI_CHAR const *const PlatformConfiguration::PLATFORM_BLACKBERRY    = "blackberry";
YI_CHAR const *const PlatformConfiguration::PLATFORM_PS3           = "ps3";
YI_CHAR const *const PlatformConfiguration::PLATFORM_PS4           = "ps4";
YI_CHAR const *const PlatformConfiguration::PLATFORM_XBOX360       = "xbox360";
YI_CHAR const *const PlatformConfiguration::PLATFORM_XBONE         = "xbone";

YI_CHAR const *const PlatformModelConfiguration::PLATFORM_MODEL_FIRE       = "fire";
YI_CHAR const *const PlatformModelConfiguration::PLATFORM_MODEL_NEXUS      = "nexus";
YI_CHAR const *const PlatformModelConfiguration::PLATFORM_MODEL_ANDROID_TV = "androidtv";

ApplicationConfiguration *LanguageConfiguration::s_applicationConfiguration = YI_NULL;
ApplicationConfiguration *AspectRatioConfiguration::s_applicationConfiguration = YI_NULL;
ApplicationConfiguration *FormFactorConfiguration::s_applicationConfiguration = YI_NULL;
ApplicationConfiguration *PlatformConfiguration::s_applicationConfiguration = YI_NULL;
ApplicationConfiguration *PlatformModelConfiguration::s_applicationConfiguration = YI_NULL;

ApplicationConfiguration & ApplicationConfiguration::GetInstance()
{
    static ApplicationConfiguration applicationConfiguration;

    return applicationConfiguration;
}

ApplicationConfiguration::ApplicationConfiguration()
{
    // Add default setting for each new configuration added
    SetPlatform(CONFIGURATION_NONE);
    SetPlatformModel(CONFIGURATION_NONE);
    SetFormFactor(CONFIGURATION_NONE);
    SetScreenAspectRatio(CONFIGURATION_NONE);
    SetLanguage(CONFIGURATION_NONE);
}

ApplicationConfiguration::~ApplicationConfiguration()
{
}

void ApplicationConfiguration::RefreshAssetLoaderAssetRootLocator()
{
    CYIAssetLocator assetLocator = CYIAssetLoader::GetAssetLocator();// Note that CYIAssetLoader::GetRootLocator returns a copy
    assetLocator.Refresh();
    CYIAssetLoader::SetAssetLocator(assetLocator);
}

void ApplicationConfiguration::AddConfigurations()
{
    // Get a copy
    CYIAssetLocator assetLocator = CYIAssetLoader::GetAssetLocator();

    // Add more configurations here. The order matters.

    assetLocator.AddConfigurationProperty(new PlatformConfiguration());
    PlatformModelConfiguration::s_applicationConfiguration = this;

    assetLocator.AddConfigurationProperty(new PlatformModelConfiguration());
    PlatformConfiguration::s_applicationConfiguration = this;

    assetLocator.AddConfigurationProperty(new FormFactorConfiguration());
    FormFactorConfiguration::s_applicationConfiguration = this;

    assetLocator.AddConfigurationProperty(new LanguageConfiguration());
    LanguageConfiguration::s_applicationConfiguration = this;

    assetLocator.AddConfigurationProperty(new AspectRatioConfiguration());
    AspectRatioConfiguration::s_applicationConfiguration = this;

    assetLocator.Refresh();

    // Set our new copy
    CYIAssetLoader::SetAssetLocator(assetLocator);
}
