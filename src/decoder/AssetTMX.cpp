#include "AssetTMX.h"

YI_RTTI_DEF1_INST(AssetTMX, "AssetTMX", CYIAsset)

AssetTMX::AssetTMX() : 
    m_pTMXMap(YI_NULL)
{
}

AssetTMX::AssetTMX(const CYIString& path, PATH_TYPE ePathType) :
    m_pTMXMap(YI_NULL)
{
    SetPath(path, ePathType);
}

AssetTMX::~AssetTMX()
{
}

const CYISharedPtr<tmxparser::TmxMap> &AssetTMX::GetTMXMap()
{
    if (!IsLoaded())
    {
        Load();
    }

    return m_pTMXMap;
}

void AssetTMX::SetTMXMap(const CYISharedPtr<tmxparser::TmxMap> &map)
{
    m_pTMXMap = map;
}

void AssetTMX::SetTMXMap(tmxparser::TmxMap *map)
{
    m_pTMXMap = CYISharedPtr<tmxparser::TmxMap>(map);
}

YI_UINT32 AssetTMX::GetTilesetCount() const
{
    if (m_pTMXMap)
    {
        return m_pTMXMap->tilesetCollection.size();
    }

    return 0;
}

CYISharedPtr<CYIAssetTextureBase> AssetTMX::GetTilesetTexture(YI_UINT32 tilesetIndex)
{
    if (tilesetIndex < m_pTMXMap->tilesetCollection.size())
    {
        return m_pTilesetsTextures[tilesetIndex];
    }

    return CYISharedPtr<CYIAssetTextureBase>(YI_NULL);
}

void AssetTMX::RemoveTilesets()
{
    // TODO look into this method.
    // I wouldn't have expected this to crash... 
//    std::vector<CYISharedPtr<CYIAssetTextureBase> >::iterator it = m_pTilesetsTextures.begin();
//    std::vector<CYISharedPtr<CYIAssetTextureBase> >::iterator end = m_pTilesetsTextures.end();
//
//    while(it != end)
//    {
//        if (!it->IsNull())
//        {
//            m_pTilesetsTextures.erase(it);
//        }
//        ++it;
//    }

    m_pTilesetsTextures.clear();
}

void AssetTMX::AddTilesetTexture(CYISharedPtr<CYIAssetTextureBase> pAssetTexture)
{
    m_pTilesetsTextures.push_back(pAssetTexture);
}

