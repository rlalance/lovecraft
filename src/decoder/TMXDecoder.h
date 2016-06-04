#pragma once

#include "decoder/AssetTMX.h"
#include <asset/YiAssetDecoder.h>

class TMXDecoder : public CYIAssetDecoder
{
public:
    TMXDecoder();
    ~TMXDecoder();
    bool SupportsParams(const CYIAssetLoadParams* pDecodeParams);
    CYISharedPtr<CYIAsset> DecodeAsset(const CYIString& path, const CYIAssetLoadParams* pAssetParams);
    CYISharedPtr<CYIAsset> DecodeAsset(const YI_UINT8* pData, YI_UINT32 nDataSize, const CYIAssetLoadParams* pAssetParams);
    bool DecodeAsset(const CYISharedPtr<CYIAsset>& pAsset);
    bool IsFormatSupported(const YI_UINT8* pData, YI_UINT32 nDataSize);

    bool PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const CYIString& path, const CYIAssetLoadParams* pDecodeParams) const;
    bool PopulateTMX(const CYISharedPtr<AssetTMX> &pAsset, const YI_UINT8* pData, YI_UINT32 nDataSize, const CYIAssetLoadParams* pDecodeParams) const;
    void LoadTMXMapTilesets(const CYISharedPtr<AssetTMX> &pAssetTMX, const tmxparser::TmxMap* pMap, const CYIString &path) const;

    YI_TYPE_DATA
};

