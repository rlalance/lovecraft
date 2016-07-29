#ifndef _NOTOFICATION_VIEW_H_
#define _NOTOFICATION_VIEW_H_

#include <animation/YiAnimationToggleController.h>
#include <view/YiSceneView.h>

/*!
 \Template for a After Effects composition representing a notification.
 Base class for full screen and popup notifications brief
 
 <B>Notification Template Specification </B>
 
 | Type      | Labels                   | Property          | Description                                                   |
 | :-------- | :----------------------- | :---------------: | :-----------------------------------------------------------  |
 | Node      | Placeholder-Title        | <B>Required</B>   | ill contain the notification-specified message title          |
 | Node      | Placeholder-Description  | <B>Required</B>   | ill contain the notification-specified message description    |
 */

class NotificationView : public CYISceneView
{
public:
    NotificationView();
    virtual ~NotificationView();
    
    virtual bool Init();
    
    void SetSceneName(const CYIString &sceneName);
    const CYIString &GetSceneName() const;
    
    void SetTitle(const CYIString &text);
    void SetMessage(const CYIString &text);
    
    void ShowOverlay();
    void HideOverlay();
    bool IsOverlayShown();
    
    CYISignal<NotificationView *> Dismissed;
    CYISignal<NotificationView *> AnimationdOutEnded;
    
protected:
    void DoDismiss();
    void AnimationdOutEnd();
    
private:
    void SetMessageText(const CYIString &textField, const CYIString &text);
    
    CYIString m_sceneName;
    CYIAnimationToggleController m_diplayAnimationController;
    
    YI_DISALLOW_COPY_AND_ASSIGN(NotificationView)
    YI_TYPE_DATA
};

#endif // _NOTOFICATION_VIEW_H_