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

