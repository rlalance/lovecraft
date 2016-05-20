#ifndef __APPLICATIONCONFIGURATION_H_
#define __APPLICATIONCONFIGURATION_H_

#include <asset/YiAbstractAssetConfiguration.h>
#include <framework/YiScreen.h>
#include <utility/YiString.h>

/*! \addtogroup asset-root-locator-sample

    This sample contains custom configuration qualifiers.

    - The order the qualifiers are processed, depends on the order they are added to the CYIAssetRootLocator.
    - It is preferable to build the folder names in the same order they are added to the CYIAssetRootLocator, but not mandatory.
    - Qualifiers must be unique, including the qualifiers in you.i engine.
    - Qualifiers should be short since they are used in building folder names. The folder name should not exceed 255.
    - Qualifiers must not included the reserved character '-' since it is used to delimit them.
    - Qualifiers should comply to OS specific file system requirements.
    To ensure maximum portability, it is suggested to use only the following characters: a-z A-Z 0-9 _

    \code{.cpp}
    class AssetRootLocatorSampleApp : public CYIApp, public CYIEventHandler
    {
        ...

    private:
        ApplicationConfiguration s_applicationConfiguration;
        ...
    }

    ...

    YI_BOOL AssetRootLocatorSampleApp::UserInit()
    {
        s_applicationConfiguration.AddConfigurations();
        ...
    }

    void AssetRootLocatorSampleApp::TestConfiguration()
    {
        // Configure the asset root locator for:
        // android platform, no model specified, 16x9 aspect ratio, using watch form factor and staging server configuration
        s_applicationConfiguration.SetPlatform(PlatformConfiguration::PLATFORM_ANDROID);
        s_applicationConfiguration.SetPlatformModel(ApplicationConfiguration::CONFIGURATION_NONE);
        s_applicationConfiguration.SetScreenAspectRatio(AspectRatioConfiguration::SCREEN_ASPECT_RATIO_16x9);
        s_applicationConfiguration.SetFormFactor(FORM_FACTOR_WATCH);
        s_applicationConfiguration.SetLanguage(LANGUAGE_SPANISH_ARGENTINA);
        s_applicationConfiguration.RefreshAssetLoaderAssetRootLocator();
    }

    \endcode

    \warning Only some of the configuration were exercised when building the sample app. Test all of these configuration within the context of your app.
    \warning Due to some you.i engine limitations, dynamically changing a configuration once the app is fully "booted" might result in unexpected behavior.
    To work around this limitation, ensure that your configuration is completed inside App::UserInit.
  @{
 */


class ApplicationConfiguration
{
public:
    static char const *const CONFIGURATION_NONE;

public:
    ApplicationConfiguration();
    virtual ~ApplicationConfiguration();

    static ApplicationConfiguration & GetInstance();

    void AddConfigurations();

    CYIString const &GetLanguage() const
    {
        return m_language;
    }

    void SetLanguage(CYIString const &language)
    {
        m_language = language;
    }

    CYIString const &GetFormFactor() const
    {
        return m_formFactor;
    }

    void SetFormFactor(CYIString const &formFactor)
    {
        m_formFactor = formFactor;
    }

    CYIString const &GetPlatform() const
    {
        return m_platform;
    }

    void SetPlatform(CYIString const &platform)
    {
        m_platform = platform;
    }

    CYIString const &GetPlatformModel() const
    {
        return m_platformModel;
    }

    void SetPlatformModel(CYIString const &platformModel)
    {
        m_platformModel = platformModel;
    }

    CYIString const &GetScreenAspectRatio() const
    {
        return m_screenAspectRatio;
    }

    void SetScreenAspectRatio(CYIString const &screenAspectRatio)
    {
        m_screenAspectRatio = screenAspectRatio;
    }

    CYIString const &GetDataPath() const
    {
        return m_dataPath;
    }

    void SetDataPath(CYIString const &dataPath)
    {
        m_dataPath = dataPath;
    }

    YI_FLOAT GetScreenScaleFactor() const
    {
        return m_fScaleFactor;
    }

    void SetScreenScaleFactor(YI_FLOAT fScaleFactor)
    {
        m_fScaleFactor = fScaleFactor;
    }

    void RefreshAssetLoaderAssetRootLocator();

private:
    CYIString m_language;
    CYIString m_formFactor;
    CYIString m_platform;
    CYIString m_platformModel;
    CYIString m_screenAspectRatio;
    CYIString m_dataPath;
    YI_FLOAT m_fScaleFactor;
};

class LanguageConfiguration : public CYIAbstractAssetConfiguration
{
public:
    static char const *const LANGUAGE_SPANISH_ARGENTINA;
    static char const *const LANGUAGE_PORTUGUESE_BRAZIL;
    static char const *const LANGUAGE_ENGLISH_CANADA;

public:
    LanguageConfiguration()
    {
        m_qualifierValues.push_back(LANGUAGE_SPANISH_ARGENTINA);
        m_qualifierValues.push_back(LANGUAGE_PORTUGUESE_BRAZIL);
        m_qualifierValues.push_back(LANGUAGE_ENGLISH_CANADA);
    }

    virtual ~LanguageConfiguration()
    {
    }

    virtual LanguageConfiguration *Clone() const
    {
        return new LanguageConfiguration(*this);
    }

    YI_UINT32 Test(const CYIString language) const
    {
        if(s_applicationConfiguration && language == s_applicationConfiguration->GetLanguage())
        {
            return 1;
        }
        return 0;
    }

    static ApplicationConfiguration *s_applicationConfiguration;
};

class AspectRatioConfiguration : public CYIAbstractAssetConfiguration
{
public:
    static char const *const SCREEN_ASPECT_RATIO_1x1;
    static char const *const SCREEN_ASPECT_RATIO_5x4;
    static char const *const SCREEN_ASPECT_RATIO_4x3;
    static char const *const SCREEN_ASPECT_RATIO_3x2;
    static char const *const SCREEN_ASPECT_RATIO_8x5;
    static char const *const SCREEN_ASPECT_RATIO_5x3;
    static char const *const SCREEN_ASPECT_RATIO_16x9;
    static char const *const SCREEN_ASPECT_RATIO_17x9;
    static char const *const SCREEN_ASPECT_RATIO_21x9;
    static char const *const SCREEN_ASPECT_RATIO_256x135;

public:
    AspectRatioConfiguration()
    {
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_1x1);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_5x4);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_4x3);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_3x2);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_8x5);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_5x3);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_16x9);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_17x9);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_21x9);
        m_qualifierValues.push_back(SCREEN_ASPECT_RATIO_256x135);
    }

    virtual ~AspectRatioConfiguration()
    {
    }

    virtual AspectRatioConfiguration *Clone() const
    {
        return new AspectRatioConfiguration(*this);
    }

    YI_UINT32 Test(const CYIString screenAspectRatio) const
    {
        if(s_applicationConfiguration && screenAspectRatio == s_applicationConfiguration->GetScreenAspectRatio())
        {
            return 1;
        }
        return 0;
    }

    static ApplicationConfiguration *s_applicationConfiguration;
};

class FormFactorConfiguration : public CYIAbstractAssetConfiguration
{
public:
    static char const *const FORM_FACTOR_HANDSET;
    static char const *const FORM_FACTOR_TABLET;
    static char const *const FORM_FACTOR_CAR;
    static char const *const FORM_FACTOR_DESKTOP;
    static char const *const FORM_FACTOR_TV;
    static char const *const FORM_FACTOR_APPLIANCE;
    static char const *const FORM_FACTOR_WATCH;

public:
    FormFactorConfiguration()
    {
        m_qualifierValues.push_back(FORM_FACTOR_HANDSET);
        m_qualifierValues.push_back(FORM_FACTOR_TABLET);
        m_qualifierValues.push_back(FORM_FACTOR_CAR);
        m_qualifierValues.push_back(FORM_FACTOR_DESKTOP);
        m_qualifierValues.push_back(FORM_FACTOR_TV);
        m_qualifierValues.push_back(FORM_FACTOR_APPLIANCE);
        m_qualifierValues.push_back(FORM_FACTOR_WATCH);
    }

    virtual ~FormFactorConfiguration()
    {
    }

    virtual FormFactorConfiguration *Clone() const
    {
        return new FormFactorConfiguration(*this);
    }

    YI_UINT32 Test(const CYIString formFactor) const
    {
        if(s_applicationConfiguration && formFactor == s_applicationConfiguration->GetFormFactor())
        {
            return 1;
        }
        return 0;
    }

    static ApplicationConfiguration *s_applicationConfiguration;
};

class PlatformConfiguration : public CYIAbstractAssetConfiguration
{
public:
    static char const *const PLATFORM_ANDROID;
    static char const *const PLATFORM_ROKU;
    static char const *const PLATFORM_IOS;
    static char const *const PLATFORM_OSX;
    static char const *const PLATFORM_APPLETV;
    static char const *const PLATFORM_WINDOWS;
    static char const *const PLATFORM_WINDOWSMOBILE;
    static char const *const PLATFORM_BLACKBERRY;
    static char const *const PLATFORM_PS3;
    static char const *const PLATFORM_PS4;
    static char const *const PLATFORM_XBOX360;
    static char const *const PLATFORM_XBONE;

public:
    PlatformConfiguration()
    {
        m_qualifierValues.push_back(PLATFORM_ANDROID);
        m_qualifierValues.push_back(PLATFORM_ROKU);
        m_qualifierValues.push_back(PLATFORM_IOS);
        m_qualifierValues.push_back(PLATFORM_OSX);
        m_qualifierValues.push_back(PLATFORM_APPLETV);
        m_qualifierValues.push_back(PLATFORM_WINDOWS);
        m_qualifierValues.push_back(PLATFORM_WINDOWSMOBILE);
        m_qualifierValues.push_back(PLATFORM_BLACKBERRY);
        m_qualifierValues.push_back(PLATFORM_PS3);
        m_qualifierValues.push_back(PLATFORM_PS4);
        m_qualifierValues.push_back(PLATFORM_XBOX360);
        m_qualifierValues.push_back(PLATFORM_XBONE);
    }

    virtual ~PlatformConfiguration()
    {
    }

    virtual PlatformConfiguration *Clone() const
    {
        return new PlatformConfiguration(*this);
    }

    YI_UINT32 Test(const CYIString platform) const
    {
        if(s_applicationConfiguration && platform == s_applicationConfiguration->GetPlatform())
        {
            return 1;
        }
        return 0;
    }

    std::list<CYIString> GetQualifierValues() const
    {
        return m_qualifierValues;
    }

    static ApplicationConfiguration *s_applicationConfiguration;
};

class PlatformModelConfiguration : public CYIAbstractAssetConfiguration
{
public:
    static char const *const PLATFORM_MODEL_FIRE;
    static char const *const PLATFORM_MODEL_NEXUS;
    static char const *const PLATFORM_MODEL_ANDROID_TV;

public:
    PlatformModelConfiguration()
    {
        m_qualifierValues.push_back(PLATFORM_MODEL_FIRE);
        m_qualifierValues.push_back(PLATFORM_MODEL_NEXUS);
        m_qualifierValues.push_back(PLATFORM_MODEL_ANDROID_TV);
    }

    virtual ~PlatformModelConfiguration()
    {
    }

    virtual PlatformModelConfiguration *Clone() const
    {
        return new PlatformModelConfiguration(*this);
    }

    YI_UINT32 Test(const CYIString platformModel) const
    {
        if(s_applicationConfiguration && platformModel == s_applicationConfiguration->GetPlatformModel())
        {
            return 1;
        }
        return 0;
    }

    static ApplicationConfiguration *s_applicationConfiguration;
};
/*!
@}
*/

#endif
