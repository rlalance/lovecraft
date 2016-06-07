#include "TMXLayerNode.h"
#include "utilities/MeshFactory.h"
#include "utilities/tmxparser.h"

#include <asset/YiAssetManager.h>
#include <framework/YiFramework.h>
#include <renderer/YiMaterial.h>

YI_RTTI_DEF1_INST(TMXLayerNode, "TMXLayerNode", CYISceneNode);

static const CYIString TAG = "TMXLayerNode";

struct TILE
{
    struct POSITION
    {
        YI_FLOAT x;
        YI_FLOAT y;
        YI_FLOAT z;
    };

    struct TEXTCOORD
    {
        YI_FLOAT s;
        YI_FLOAT t;
    };

    struct TILEVERTEX
    {
        POSITION position;
        TEXTCOORD texcoord;
    };

    TILEVERTEX vertices[4];
};

TMXLayerNode::TMXLayerNode()
{
}

TMXLayerNode::~TMXLayerNode()
{
}

/*
        c0       c1
    0------1,4-----5,
    |       |       |
r0  |       |       |
    |       |       |
    2______3,6 _____7
    8      9,10   11,
    |       |       |
r1  |       |       |
    |       |       |
    12-----13,14---15
*/
void TMXLayerNode::SetMeshTileUV(CYISharedPtr<CYIMesh> pMesh, const tmxparser::TmxLayerTile &tmxLayerTile, const tmxparser::TmxTileset &tmxTileset)
{
    // const TmxTileset &tileset, unsigned int tileFlatIndex, float pixelCorrection, bool flipY, TmxRect &outRect
    tmxparser::TmxRect outRect;
    tmxparser::calculateTileCoordinatesUV(tmxTileset, tmxLayerTile.tileFlatIndex, 0.5f, false, outRect);

    CYISharedPtr<CYIAssetBufferObjectData> pVBO = pMesh->GetVertexBuffer(0);

    const CYIMesh::YI_ATTRIBUTE_BINDING &binding = pMesh->GetBinding(CYIMesh::YI_POSITION);
    YI_UINT32 uByteOffset = binding.uByteOffset;
    std::vector<YI_UINT8> pBuf = pVBO->GetData();
    TILE* tile = reinterpret_cast<TILE*>(pBuf.data() + uByteOffset);

    tile += tmxLayerTile.tileFlatIndex;

    tile->vertices[0].texcoord.s = outRect.u;
    tile->vertices[0].texcoord.t = outRect.v;

    tile->vertices[1].texcoord.s = outRect.u2;
    tile->vertices[1].texcoord.t = outRect.v;

    tile->vertices[2].texcoord.s = outRect.u;
    tile->vertices[2].texcoord.t = outRect.v2;

    tile->vertices[3].texcoord.s = outRect.u2;
    tile->vertices[3].texcoord.t = outRect.v2;

    pVBO->SetData(pBuf);
}

void TMXLayerNode::Create(CYISharedPtr<tmxparser::TmxMap> pTmxMap, const tmxparser::TmxLayer &tmxLayer, CYISharedPtr<CYIAssetTextureBase> pLayerTexture)
{
    CYISharedPtr<CYIMesh> pMesh = MeshFactory::CreateGridMesh(tmxLayer.width, tmxLayer.height, pTmxMap->tileWidth*5, pTmxMap->tileHeight*5);

    CYISharedPtr<CYIMaterial> pMaterial = CYISharedPtr<CYIMaterial>(new CYIMaterial);

    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();
    CYISharedPtr<CYIAssetShaderProgram> pShaderAsset = pAM->GetAsset(CYIAssetManager::YI_PROGRAM_3D_RGB).DynamicCast<CYIAssetShaderProgram>();
    pMaterial->SetShaderProgram(pShaderAsset);
    pMaterial->SetTexture(0, pLayerTexture);

    SetMaterial(pMaterial);
    SetMesh(pMesh);

    tmxparser::TmxTileset tmxTileset = pTmxMap->tilesetList[0];

    // Set the proper UVs now...
    for (YI_UINT32 tileIndex = 0; tileIndex < tmxLayer.tiles.size(); tileIndex++)
    {
        tmxparser::TmxLayerTile tile = tmxLayer.tiles[tileIndex];

        SetMeshTileUV(pMesh, tile, tmxTileset);
    }
}
