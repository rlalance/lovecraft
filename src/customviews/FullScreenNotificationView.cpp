#include "customviews/FullScreenNotificationView.h"

#include "utilities/NodeUtilities.h"

#include <view/YiPushButtonView.h>
#include <scenetree/YiTextSceneNode.h>

const CYIString TAG("FullScreenNotificationView.h");

YI_RTTI_DEF1_INST(FullScreenNotificationView, "FullScreenNotificationView", CYISceneView)

FullScreenNotificationView::FullScreenNotificationView()
{
};

FullScreenNotificationView::~FullScreenNotificationView()
{
};

bool FullScreenNotificationView::Init()
{
    return NotificationView::Init();
}


void FullScreenNotificationView::ConnectButtons()
{
    Utility::GetNode<CYIPushButtonView>(this, "Btn-Action1")->ButtonReleased.Connect(*this, &FullScreenNotificationView::DoAction);
    Utility::GetNode<CYIPushButtonView>(this, "Btn-Action2")->ButtonReleased.Connect(*this, &FullScreenNotificationView::DoDismiss);
}

void FullScreenNotificationView::DoAction()
{
    Actioned(this);
}

void FullScreenNotificationView::DoDismiss()
{
    NotificationView::DoDismiss();
}

void FullScreenNotificationView::SetActionButtonText(const CYIString &text)
{
    SetButtonText("Btn-Action1", text);
}

void FullScreenNotificationView::SetDismissButtonText(const CYIString &text)
{
   SetButtonText("Btn-Action2", text);
}

void FullScreenNotificationView::SetButtonText(const CYIString &button, const CYIString &text)
{
    CYIPushButtonView *pButton = Utility::GetNode<CYIPushButtonView>(this, button);
    
    if(text.IsNotEmpty())
    {
        pButton->Show();
        pButton->SetText(text);
        Utility::GetNode<CYITextSceneNode>(pButton, "Placeholder-Title")->SetText(text);
    }
    else
    {
        pButton->Hide();
    }
}
