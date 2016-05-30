#include "TMXDecoder.h"

#include "decoder/AssetTMX.h"
#include "utilities/tmxparser.h"

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

bool TMXDecoder::PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const CYIString &path, const CYIAssetLoadParams *)
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
    }
    else
    {
        delete pMap;
        bParsingSucceeded = true;
    }

    return bParsingSucceeded;
}

bool TMXDecoder::PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const YI_UINT8 *pData, YI_UINT32 nDataSize, const CYIAssetLoadParams *pDecodeParams)
{
    YI_UNUSED(pAsset);
    YI_UNUSED(pData);
    YI_UNUSED(nDataSize);
    YI_UNUSED(pDecodeParams);

    return false;
}
