// © You i Labs Inc. 2000-2016. All rights reserved.
#include "EmptySampleApp.h"

#include <framework/YiFramework.h>
#include <scenetree/YiSceneManager.h>
#include <view/YiSceneView.h>


#include <datamodel/YiAbstractDataModel.h>
#include "quest/QuestModel.h"
#include "quest/QuestParser.h"

/*! \addtogroup empty-sample
  @{
 */

EmptySampleApp::EmptySampleApp()
{
}

EmptySampleApp::~EmptySampleApp()
{
}

bool EmptySampleApp::UserInit()
{
    // Load a layout file which will be the root scene view.
    CYISceneView *pSceneViewMain = GetSceneManager()->LoadScene("EmptySample_MainComp.layout", CYISceneManager::SCALE_STRETCH, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);
    if(!pSceneViewMain)
    {
        YI_LOGE("EmptySampleApp::UserInit", "Loading scene has failed");
        return false;
    }
    // Add the scene view to the scene manager.
    GetSceneManager()->AddScene("MainComp", pSceneViewMain, 0, CYISceneManager::LAYER_OPAQUE);

    return true;
}

bool EmptySampleApp::UserStart()
{
    // This is the first function called upon completion of engine and application initialization. Return false in case of failure.
	

	QuestModel* testQuest = QuestParser::ParseQuest("Resources/TestQuest.json");

	CYIString testQuestString = testQuest->ToString();
    
	return true;
}

void EmptySampleApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}

/*!
@}
*/

