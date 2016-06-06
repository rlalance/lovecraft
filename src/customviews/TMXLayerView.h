#ifndef TMXLAYERVIEW_H
#define TMXLAYERVIEW_H

#include <view\YiSceneView.h>
#include <smartptr/YiEnableSharedFromThis.h>
#include <utilities/tmxparser.h>

class TMXLayerView :
    public CYISceneView
{
public:
    TMXLayerView();
    ~TMXLayerView();
    void Create(CYISharedPtr<tmxparser::TmxMap> pTmxMap, const tmxparser::TmxLayer &tmxLayer);

private:
    YI_DISALLOW_COPY_AND_ASSIGN(TMXLayerView);
    YI_TYPE_DATA
};

#endif
