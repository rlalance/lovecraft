#ifndef TMXLAYERNODE_H
#define TMXLAYERNODE_H

#include "utilities/tmxparser.h"

#include <view\YiSceneView.h>
#include <asset/YiAssetTextureBase.h>

class TMXLayerNode : public CYISceneNode
{
public:
    TMXLayerNode();
    ~TMXLayerNode();
    void SetMeshTileUV(CYISharedPtr<CYIMesh> pMesh, const tmxparser::TmxLayerTile &tile, const tmxparser::TmxTileset &tmxTileset);
    void Create(CYISharedPtr<tmxparser::TmxMap> pTmxMap, const tmxparser::TmxLayer &tmxLayer, CYISharedPtr<CYIAssetTextureBase> pLayerTexture);

private:
    YI_DISALLOW_COPY_AND_ASSIGN(TMXLayerNode);
    YI_TYPE_DATA
};

#endif
