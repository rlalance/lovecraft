#pragma once

#include "decoder/AssetTMX.h"
#include "utilities/tmxparser.h"

#include <asset/YiAssetDecoder.h>
#include <asset/YiAssetTexture.h>
#include <smartptr/YiSharedPtr.h>

class AssetTMX : public CYIAsset
{
public:
    AssetTMX();
    AssetTMX(const CYIString& path, PATH_TYPE ePathType);
    ~AssetTMX();

    const CYISharedPtr<tmxparser::TmxMap> &GetTMXMap();
    void SetTMXMap(const CYISharedPtr<tmxparser::TmxMap> &map);
    void SetTMXMap(tmxparser::TmxMap *map);
    YI_UINT32 GetTilesetCount() const;
    CYISharedPtr<CYIAssetTextureBase> GetTilesetTexture(YI_UINT32 tilesetIndex);
    void RemoveTilesets();
    void AddTilesetTexture(CYISharedPtr<CYIAssetTextureBase> pAssetTexture);

private:
    CYISharedPtr<tmxparser::TmxMap> m_pTMXMap;
    std::vector<CYISharedPtr<CYIAssetTextureBase> > m_pTilesetsTextures;

    YI_TYPE_DATA
};