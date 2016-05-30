#include "TMXView.h"
#include <utilities/tmxparser.h>

YI_RTTI_DEF1_INST(TMXView, "TMXView", CYISceneView);

static const CYIString TAG = "TMXView"; 

TMXView::TMXView()
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

        LoadTMX();
    }

    return bInit;
}

bool TMXView::LoadTMX()
{
    //tmxparser::TmxReturn error;
    //tmxparser::TmxMap map;

    //// test from file
    //CYIString assetPath = GetAssetsPath();

    //CYIString levelPath = assetPath + "resources/" + "test_xml_level.tmx";

    //error = tmxparser::parseFromFile(levelPath.GetData(), &map, assetPath.GetData());
    return false;
}
