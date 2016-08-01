#include "customviews/PopupNotificationView.h"

#include "utilities/NodeUtilities.h"

#include <scenetree/YiTextSceneNode.h>
#include <view/YiPushButtonView.h>

const CYIString TAG("PopupNotificationView.h");

YI_RTTI_DEF1_INST(PopupNotificationView, "PopupNotificationView", CYISceneView)

PopupNotificationView::PopupNotificationView()
{
};

PopupNotificationView::~PopupNotificationView()
{
};

bool PopupNotificationView::Init()
{
    return NotificationView::Init();
}

void PopupNotificationView::SetShortTitle(const CYIString &text)
{
    CYITextSceneNode *pTitle = Utility::GetNode<CYITextSceneNode>(this, "Placeholder-Title");
    
    if (pTitle != YI_NULL)
    {
        pTitle->SetText(text);
    }
}

void PopupNotificationView::ConnectButtons()
{
    CYIPushButtonView *pCloseButton = Utility::GetNode<CYIPushButtonView>(this, "Btn-Close", Utility::GET_NODE_FLAG_OPTIONAL);

    if(pCloseButton)
    {
        pCloseButton->ButtonReleased.Connect(*this, &PopupNotificationView::DoDismiss);
    }
}

void PopupNotificationView::DoDismiss()
{
    NotificationView::DoDismiss();
}

