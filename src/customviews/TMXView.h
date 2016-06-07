#ifndef TMXVIEW_H
#define TMXVIEW_H

#include "decoder/AssetTMX.h"

#include <view\YiSceneView.h>

/*! 
    TMXView is the parent of TMXLayerNode's.
*/
class TMXView : public CYISceneView
{
public:
    TMXView();
    ~TMXView();

    virtual bool Init();
    CYISharedPtr<AssetTMX> LoadTMXAsset(CYIString assetFilename);

private:
    CYISharedPtr<AssetTMX> m_pAssetTMX;

    YI_DISALLOW_COPY_AND_ASSIGN(TMXView);
    YI_TYPE_DATA
};

#endif