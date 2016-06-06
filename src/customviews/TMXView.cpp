#include "TMXView.h"

#include "customviews/TMXLayerView.h"

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

        m_pAssetTMX = LoadTMXAsset(assetFilename);

        CYISharedPtr<tmxparser::TmxMap> m_pTMXMap = m_pAssetTMX->GetTMXMap();
        tmxparser::TmxLayerCollection_t &layerCollection = m_pTMXMap->layerCollection;

        for (YI_UINT32 layerIndex = 0; layerIndex < layerCollection.size(); layerIndex++)
        {
            tmxparser::TmxLayer &tmxLayer = layerCollection[layerIndex];

            //TODO put this back in when ready
            //TMXLayerView *pTmxLayerView = new TMXLayerView();
            //pTmxLayerView->Init();
            //pTmxLayerView->Create(m_pTMXMap, tmxLayer);
            //AddChild(pTmxLayerView);
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
