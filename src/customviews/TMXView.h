#ifndef TMXVIEW_H
#define TMXVIEW_H

#include "decoder/AssetTMX.h"

#include <view\YiSceneView.h>

class TMXView : public CYISceneView
{
public:
    TMXView();
    ~TMXView();

    virtual bool Init();
    bool LoadTMXAsset(CYIString assetFilename);

private:
    CYISharedPtr<AssetTMX> m_pAssetTMX;

    YI_DISALLOW_COPY_AND_ASSIGN(TMXView);
    YI_TYPE_DATA
};

#endif