#include "app/AppController.h"

#include <framework/YiApp.h>
#include <state/YiSignalTransition.h>

static const CYIString TAG = "AppController";

const YI_UINT64 APP_CONFIG_FETCH_TIMEOUT_MS = 10000;
const YI_UINT64 APP_INIT_RETRY_TIMER_MS = 5000;

AppController::AppController(CYIApp &rApp)
    : m_rApp(rApp)
    , m_navigationController(m_rApp)
    , m_notificationController(m_rApp)
{
    BuildStateMachine();
}

AppController::~AppController()
{
    if (m_StateMachine.IsRunning())
    {
        m_StateMachine.Stop();
    }
}

void AppController::BuildStateMachine()
{
    // ConfigInit
    CYIState *pConfigInitState = new CYIState(&m_StateMachine, "ConfigInit");
    pConfigInitState->StateEntered.Connect(*this, &AppController::ConfigInitStateEntered);
    pConfigInitState->StateExited.Connect(*this, &AppController::ConfigInitStateExited);
    
    // Running
    CYIState *pRunningState = new CYIState(&m_StateMachine, "Running");
    pRunningState->StateEntered.Connect(*this, &AppController::RunningStateEntered);
    pRunningState->StateExited.Connect(*this, &AppController::RunningStateExited);

    new CYISignalTransition<>(&TransitionFromConfigInitToRunning, pConfigInitState, pRunningState);

    m_StateMachine.SetInitialState(pConfigInitState);
}

bool AppController::Start()
{
    return m_StateMachine.Start();
}

bool AppController::HandleEvent(const CYISharedPtr<CYIEventDispatcher> &pDispatcher, CYIEvent *pEvent)
{
    return false;
}

void AppController::ConfigInitStateEntered()
{
    YI_LOGD(TAG, "ConfigInitStateEntered");

    m_notificationController.Init();

    TransitionFromConfigInitToRunning();
}

void AppController::ConfigInitStateExited()
{
    YI_LOGD(TAG, "ConfigInitStateExited");
}

void AppController::RunningStateEntered()
{
    YI_LOGD(TAG, "RunningStateEntered");

    m_navigationController.Start(NavigationController::BOOK);
}

void AppController::RunningStateExited()
{
    YI_LOGD(TAG, "RunningStateExited");
}

void AppController::ConfigInitToRunningTransitionStarted()
{
    YI_LOGD(TAG, "ConfigInitToRunningTransitionStarted");
}

void AppController::ConfigInitToRunningTransitionEnded()
{
    YI_LOGD(TAG, "ConfigInitToRunningTransitionEnded");
}
