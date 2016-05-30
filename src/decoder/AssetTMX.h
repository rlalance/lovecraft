#pragma once

#include "decoder/AssetTMX.h"
#include "utilities/tmxparser.h"

#include <asset/YiAssetDecoder.h>
#include <smartptr/YiSharedPtr.h>

class AssetTMX : public CYIAsset
{
public:
    AssetTMX();
    AssetTMX(const CYIString& path, PATH_TYPE ePathType);
    ~AssetTMX();

    const CYISharedPtr<tmxparser::TmxMap> &GetTMXMap();
    void SetTMXMap(const CYISharedPtr<tmxparser::TmxMap> &map);
    void SetTMXMap(tmxparser::TmxMap map);

private:
    CYISharedPtr<tmxparser::TmxMap> m_pTMXMap;

    YI_TYPE_DATA
};