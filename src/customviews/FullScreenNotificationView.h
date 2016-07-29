#ifndef _FULLSCREEN_NOTIFICATION_VIEW_H_
#define _FULLSCREEN_NOTIFICATION_VIEW_H_

#include "customviews/NotificationView.h"

/*!
 \Template for a After Effects composition representing a full screen notification.
 
 <B>Full Screen Notification Template Specification </B>
 
 | Type      | Labels                   | Property          | Description                                                       |
 | :-------- | :----------------------- | :---------------: | :---------------------------------------------------------------  |
 | Marker    | In                       | <B>Required</B>   | representing the animations to open the full screen notification  |
 | Marker    | Out                      | <B>Required</B>   | representing the animations to close the full screen notification |
 | Node      | Btn-Action1              | <B>Required</B>   | When pressed, the signal Actioned() is emitted                    |
 | Node      | Btn-Action2              | <B>Required</B>   | When pressed, the signal Dismissed() is emitted                   |
 */

class FullScreenNotificationView : public NotificationView
{
public:
    FullScreenNotificationView();
    virtual ~FullScreenNotificationView();
    
    virtual bool Init();
    
    void SetActionButtonText(const CYIString &text);
    void SetDismissButtonText(const CYIString &text);
    
    void ConnectButtons();

    void DoAction();
    void DoDismiss();
    
    CYISignal<FullScreenNotificationView *> Actioned;
    
private:
    void SetButtonText(const CYIString &button, const CYIString &text);
    
    YI_DISALLOW_COPY_AND_ASSIGN(FullScreenNotificationView)
    YI_TYPE_DATA
};

#endif // _FULLSCREEN_NOTIFICATION_VIEW_H_
