#include "TMXLayerView.h"
#include "utilities/MeshFactory.h"

#include <asset/YiAssetManager.h>
#include <framework/YiFramework.h>
#include <renderer/YiMaterial.h>

YI_RTTI_DEF1_INST(TMXLayerView, "TMXLayerView", CYISceneView);

static const CYIString TAG = "TMXLayerView";

TMXLayerView::TMXLayerView()
{
}


TMXLayerView::~TMXLayerView()
{
}

void TMXLayerView::Create(CYISharedPtr<tmxparser::TmxMap> pTmxMap, const tmxparser::TmxLayer &tmxLayer)
{
    CYISharedPtr<CYIMesh> pMesh = MeshFactory::CreateGridMesh(tmxLayer.width, tmxLayer.height, pTmxMap->tileWidth, pTmxMap->tileHeight);

    CYISharedPtr<CYIMaterial> pMaterial = CYISharedPtr<CYIMaterial>(new CYIMaterial);

    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();
    CYISharedPtr<CYIAssetShaderProgram> pShaderAsset = pAM->GetAsset(CYIAssetManager::YI_PROGRAM_3D_RGB).DynamicCast<CYIAssetShaderProgram>();
    pMaterial->SetShaderProgram(pShaderAsset);
    //pMaterial->SetTexture(0, m_pAssetTMX->GetTilesetTexture(0));

    SetMaterial(pMaterial);
    SetMesh(pMesh);

    // Set the proper UVs now...
    for (YI_UINT32 tileIndex = 0; tileIndex < tmxLayer.tiles.size(); tileIndex++)
    {
        tmxparser::TmxLayerTile tile = tmxLayer.tiles[tileIndex];

        //TODO
    }
}
