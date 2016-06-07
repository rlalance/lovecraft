#include "TMXDecoder.h"

#include "decoder/AssetTMX.h"
#include "utilities/tmxparser.h"

#include <asset/YiAssetManager.h>
#include <asset/YiAssetTextureBase.h>
#include <framework/YiFramework.h>
#include <logging/YiLoggerCommon.h>
#include <renderer/YiTextureFactory.h>

static const CYIString LOG_TAG("TMXDecoder");

YI_RTTI_DEF_INST(TMXDecoder, "TMXDecoder")

TMXDecoder::TMXDecoder()
{
    m_AssetType = YiGetTypeId<AssetTMX>();

    m_uSupportedExtensions = 1;
    m_fileExtensions = new CYIString[m_uSupportedExtensions];
    m_fileExtensions[0] = "tmx";
}

TMXDecoder::~TMXDecoder()
{
}

bool TMXDecoder::SupportsParams(const CYIAssetLoadParams *pDecodeParams)
{
    // no additional paramaters supported
    if (!pDecodeParams)
    {
        return true;
    }

    return false;
}

CYISharedPtr<CYIAsset> TMXDecoder::DecodeAsset(const CYIString &path, const CYIAssetLoadParams *pAssetParams)
{
    CYISharedPtr<AssetTMX> pAsset(new AssetTMX(path, CYIAsset::PATH_ABSOLUTE));

    if (!PopulateTMX(pAsset, path, pAssetParams))
    {
        return CYISharedPtr<CYIAsset>(YI_NULL);
    }

    return pAsset;
}

CYISharedPtr<CYIAsset> TMXDecoder::DecodeAsset(const YI_UINT8 *pData, YI_UINT32 nDataSize, const CYIAssetLoadParams *pAssetParams)
{
    YI_UNUSED(pData);
    YI_UNUSED(nDataSize);
    YI_UNUSED(pAssetParams);

    YI_LOGE(LOG_TAG, "Decoding a translation from memory is not supported");

    // this asset does not load from memory
    return CYISharedPtr<CYIAsset>(YI_NULL);
}

bool TMXDecoder::DecodeAsset(const CYISharedPtr<CYIAsset> &pAsset)
{
    CYISharedPtr<AssetTMX> pAssetTMX = pAsset.DynamicCast<AssetTMX>();

    return pAssetTMX && PopulateTMX(pAssetTMX, pAsset->GetName(), pAsset->GetLoadParameters());
}

bool TMXDecoder::IsFormatSupported(const YI_UINT8 *pData, YI_UINT32 nDataSize)
{
    YI_UNUSED(pData);
    YI_UNUSED(nDataSize);

    // no sniffing yet for this data type
    return false;
}

bool TMXDecoder::PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const CYIString &path, const CYIAssetLoadParams *) const
{
    bool bParsingSucceeded;

    tmxparser::TmxReturn error;
    tmxparser::TmxMap *pMap = new tmxparser::TmxMap();

    // test from file
    error = tmxparser::parseFromFile(path.GetData(), pMap, "");

    if (error == tmxparser::kSuccess)
    {
        pAsset->SetTMXMap(pMap);
        bParsingSucceeded = true;

        LoadTMXMapTilesets(pAsset, pMap, path);
    }
    else
    {
        delete pMap;
        bParsingSucceeded = true;
    }

    return bParsingSucceeded;
}

// TODO testing required for memory parsing
bool TMXDecoder::PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const YI_UINT8 *pData, YI_UINT32 nDataSize, const CYIAssetLoadParams *pDecodeParams) const
{
    bool bParsingSucceeded;

    tmxparser::TmxReturn error;
    tmxparser::TmxMap *pMap = new tmxparser::TmxMap();

    // test from file
    error = tmxparser::parseFromMemory((void*)pData, nDataSize, pMap, "");

    if (error == tmxparser::kSuccess)
    {
        pAsset->SetTMXMap(pMap);
        bParsingSucceeded = true;

        LoadTMXMapTilesets(pAsset, pMap, "memory");
    }
    else
    {
        delete pMap;
        bParsingSucceeded = true;
    }

    return bParsingSucceeded;
}

void TMXDecoder::LoadTMXMapTilesets(const CYISharedPtr<AssetTMX> &pAsset, const tmxparser::TmxMap* pMap, const CYIString &path) const
{
    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();

    pAsset->RemoveTilesets();

    for (tmxparser::TmxTilesetList::const_iterator it = pMap->tilesetList.begin(); it != pMap->tilesetList.end(); ++it)
    {
        tmxparser::TmxImage tilesetImage = it->image;
        CYIString timesetImageSource = tilesetImage.source;

        CYISharedPtr<CYIAsset> pAssetTexture = pAM->GetAsset(timesetImageSource);
        CYISharedPtr<CYIAssetTextureBase> pAssetTextureBase = pAssetTexture.DynamicCast<CYIAssetTextureBase>();

        if (!pAssetTextureBase)
        {
            pAssetTextureBase = CYITextureFactory::CreateAsset(timesetImageSource);

            if (pAssetTextureBase)
            {
                pAssetTextureBase->SetName(timesetImageSource);

                if (pAM->AddAsset(pAssetTextureBase))
                {
                    pAsset->AddTilesetTexture(pAssetTextureBase);
                }
                else
                {
                    YI_LOGE(LOG_TAG, "Failed to add TMXMap [%s] tileset [%s] to asset management", path.GetData(), timesetImageSource.GetData());
                }
            }
            else
            {
                YI_LOGE(LOG_TAG, "Failed to create TMXMap [%s] tileset [%s] to asset management", path.GetData(), timesetImageSource.GetData());
            }
        }
        else
        {
            pAsset->AddTilesetTexture(pAssetTextureBase);
        }
    }
}

