#ifndef TMXVIEW_H
#define TMXVIEW_H

#include <view\YiSceneView.h>

class TMXView : public CYISceneView
{
public:
    TMXView();
    ~TMXView();

    virtual bool Init();
    bool LoadTMX();
    YI_DISALLOW_COPY_AND_ASSIGN(TMXView);
    YI_TYPE_DATA
};

#endif