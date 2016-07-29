#ifndef _APPCONTROLLER_H_
#define _APPCONTROLLER_H_

#include "notification/NotificationController.h"
#include "screen/NavigationController.h"

#include <signal/YiSignalHandler.h>
#include <state/YiStateMachine.h>

class CYISceneView;

class AppController : public CYISignalHandler
{
public:
    AppController(CYIApp &rApp);
    virtual ~AppController();

    bool Start();
    virtual bool HandleEvent(const CYISharedPtr<CYIEventDispatcher> &pDispatcher, CYIEvent *pEvent);

private:
    void BuildStateMachine();
    void ConfigInitStateEntered();
    void ConfigInitStateExited();
    void RunningStateEntered();
    void RunningStateExited();

    void ConfigInitToRunningTransitionStarted();
    void ConfigInitToRunningTransitionEnded();

    CYIApp &m_rApp;

    CYIStateMachine m_StateMachine;
    CYISignal<> TransitionFromConfigInitToRunning;

    NavigationController m_NavigationController;
    NotificationController m_notificationController;

    YI_DISALLOW_COPY_AND_ASSIGN(AppController);
};

#endif // _APPCONTROLLER_H_

