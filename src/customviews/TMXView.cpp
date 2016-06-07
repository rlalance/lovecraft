#include "TMXView.h"

#include "customviews/TMXLayerNode.h"

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

        assetFilename = "lvl01s.tmx";

        if (assetFilename.IsEmpty())
        {
            assetFilename = "drawable/default/test_xml_level.tmx";
        }
        else
        {
            assetFilename = "drawable/default/" + assetFilename;
        }

        m_pAssetTMX = LoadTMXAsset(assetFilename);

        CYISharedPtr<tmxparser::TmxMap> m_pTMXMap = m_pAssetTMX->GetTMXMap();
        tmxparser::TmxLayerList &layerCollection = m_pTMXMap->layerList;

        for (YI_UINT32 layerIndex = 0; layerIndex < 1; layerIndex++)
        //for (YI_UINT32 layerIndex = 0; layerIndex < layerCollection.size(); layerIndex++)
        {
            tmxparser::TmxLayer &tmxLayer = layerCollection[layerIndex];

            TMXLayerNode *pTMXLayerNode = new TMXLayerNode();
            pTMXLayerNode->SetSceneManager(GetSceneManager());

            pTMXLayerNode->Init();
            pTMXLayerNode->Create(m_pTMXMap, tmxLayer, m_pAssetTMX->GetTilesetTexture(0));
            AddChild(pTMXLayerNode);
        }
    }

    return bInit;
}

CYISharedPtr<AssetTMX> TMXView::LoadTMXAsset(CYIString assetFilename)
{
    CYISharedPtr<AssetTMX> pAssetTMX;

    pAssetTMX = CYIFramework::GetInstance()->GetAssetLoader()->Load(YiGetTypeId<AssetTMX>(), assetFilename, YI_NULL);

    return pAssetTMX;
}
