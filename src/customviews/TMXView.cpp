#include "TMXView.h"

#include <asset/YiAssetLoader.h>
#include <asset/YiAssetManager.h>
#include <decoder/AssetTMX.h>
#include <framework/YiFramework.h>
#include <renderer/YiMaterial.h>
#include <renderer/YiMeshFactory.h>
#include <renderer/YiRenderSystem.h>

YI_RTTI_DEF1_INST(TMXView, "TMXView", CYISceneView);

static const CYIString TAG = "TMXView"; 

TMXView::TMXView() : m_pAssetTMX(YI_NULL)
{
}

TMXView::~TMXView()
{
}

bool TMXView::Init()
{
    bool bInit = CYISceneView::Init();

    if (bInit)
    {
        SetFocusable(true);

        CYIString assetFilename;
        GetProperty("tmxFilename", &assetFilename);

        if (assetFilename.IsEmpty())
        {
            assetFilename = "drawable/default/test_xml_level.tmx";
        }
        else
        {
            assetFilename = "drawable/default/" + assetFilename;
        }

        LoadTMXAsset(assetFilename);
    }

    return bInit;
}

bool TMXView::LoadTMXAsset(CYIString assetFilename)
{
    m_pAssetTMX = CYIFramework::GetInstance()->GetAssetLoader()->Load(YiGetTypeId<AssetTMX>(), assetFilename, YI_NULL);

    CYISharedPtr<CYIMesh> pMesh = CYIRenderSystem::GetInstance()->GetMeshFactory()->CreateQuadMesh(500.0f, 500.0f, true);
    CYISharedPtr<CYIMaterial> pMaterial = CYISharedPtr<CYIMaterial>(new CYIMaterial);

    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();
    CYISharedPtr<CYIAssetShaderProgram> pShaderAsset = pAM->GetAsset(CYIAssetManager::YI_PROGRAM_3D_RGB).DynamicCast<CYIAssetShaderProgram>();
    pMaterial->SetShaderProgram(pShaderAsset);
    pMaterial->SetTexture(0, m_pAssetTMX->GetTilesetTexture(0));

    SetMaterial(pMaterial);
    SetMesh(pMesh);
    
    return m_pAssetTMX != YI_NULL;
}
