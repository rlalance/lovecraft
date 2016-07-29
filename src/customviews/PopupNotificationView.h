#ifndef _POPUP_NOTIFICATION_VIEW_H_
#define _POPUP_NOTIFICATION_VIEW_H_

#include "customviews/NotificationView.h"

/*!
 \Template for a After Effects composition representing a popup notification.
 
 <B>Popup Notification Template Specification </B>
 
 | Type      | Labels                   | Property          | Description                                                       |
 | :-------- | :----------------------- | :---------------: | :---------------------------------------------------------------  |
 | Marker    | In                       | <B>Required</B>   | representing the animations to open the full screen notification  |
 | Marker    | Out                      | <B>Required</B>   | representing the animations to close the full screen notification |
 | Node      | Btn-Close                | <B>Required</B>   | When pressed, the signal Dismissed() is emitted                   |
 */

class PopupNotificationView : public NotificationView
{
public:
    PopupNotificationView();
    virtual ~PopupNotificationView();
    
    virtual bool Init();
    
    void SetShortTitle(const CYIString &text);
    
    void ConnectButtons();
    
    void DoDismiss();
    
private:
    YI_DISALLOW_COPY_AND_ASSIGN(PopupNotificationView)
    YI_TYPE_DATA
};

#endif /* _POPUP_NOTIFICATION_VIEW_H_ */

