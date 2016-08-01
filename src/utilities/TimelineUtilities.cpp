#include "utilities/TimelineUtilities.h"

#include <animation/YiAnimationToggleController.h>
#include <animation/YiParallelTimelineGroup.h>
#include <animation/YiSerialTimelineGroup.h>
#include <animation/YiTimeInterpolators.h>
#include <asset/YiAssetViewTemplate.h>
#include <import/YiViewTemplate.h>
#include <smartptr/YiScopedPtr.h>
#include <view/YiSceneView.h>

static const CYIString TAG = "TimelineUtilities";

YI_UINT32 PropertyAnimationDefinition::DEFAULT_PROPERTY_ANIMATION_DURATION_IN_MILLISECONDS = 500;

PropertyAnimationDefinition::PropertyAnimationDefinition()
    : m_ePropType(YI_NODE_PROPERTY_UNDEFINED),
      m_fFromValue(0.0f),
      m_fToValue(1.0f),
      m_nDurationMs(PropertyAnimationDefinition::DEFAULT_PROPERTY_ANIMATION_DURATION_IN_MILLISECONDS),
      m_eTimeInterpolatorType(PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_QUADEASEIN)
{
}

PropertyAnimationDefinition::PropertyAnimationDefinition(YI_NODE_PROPERTY_TYPE propType, YI_FLOAT fFrom, YI_FLOAT fTo, YI_UINT32 nDurMs, eTimeInterpolatorType timeInterpolatorType)
    : m_ePropType(propType),
      m_fFromValue(fFrom),
      m_fToValue(fTo),
      m_nDurationMs(nDurMs),
      m_eTimeInterpolatorType(timeInterpolatorType)
{
}

CYITimelineGroup *TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms)
{
    CYITimelineGroup *pGroup = new CYIParallelTimelineGroup();
    RecursiveBuildNodeParallelTimelineGroup(pGroup, pNode, markerName, nTimelineOffsetInms);
    pGroup->Finalize();

    if (pGroup->GetTimelineCount() == 0)
    {
        YI_LOGW(TAG, "Recursive parallel timeline group for marker '%s' is empty", markerName.GetData());
    }

    return pGroup;
}

bool TimelineUtilities::HasViewFocusInTimelines(CYISceneView *pView)
{
    if (pView)
    {
         CYIScopedPtr<CYITimelineGroup> pFocusInTimelineGroup(TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(pView, "FocusIn"));
        return pFocusInTimelineGroup && pFocusInTimelineGroup->GetTimelineCount() > 0;
    }
    return false;
}

CYITimelineGroup *TimelineUtilities::RecursiveCreateNodeSerialTimelineGroup(CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms)
{
    CYITimelineGroup *pGroup = new CYISerialTimelineGroup();
    RecursiveBuildSerialTimelineGroup(pGroup, pNode, markerName, nTimelineOffsetInms);
    pGroup->Finalize();

    if (pGroup->GetTimelineCount() == 0)
    {
        YI_LOGW(TAG, "Recursive serial timeline group for marker '%s' is empty", markerName.GetData());
    }

    return pGroup;
}

void TimelineUtilities::RecursiveBuildNodeParallelTimelineGroup(CYITimelineGroup *pGroup, CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms)
{
    if (pNode || pGroup)
    {
        AddNodeTimelineToGroup(pGroup, pNode, markerName, nTimelineOffsetInms);

        for (YI_UINT32 i = 0; i < pNode->GetChildCount(); i++)
        {
            RecursiveBuildNodeParallelTimelineGroup(pGroup, pNode->GetChild(i), markerName, nTimelineOffsetInms);
        }
    }
}

void TimelineUtilities::AddNodeTimelineToGroup(CYITimelineGroup *pGroup, const CYISceneNode *pNode, const CYIString &markerName, YI_UINT64 nTimelineOffsetInms)
{
    YI_ASSERT(pNode, TAG, "pNode is null");
    YI_ASSERT(pGroup, TAG, "pGroup is null");
    YI_ASSERT(!markerName.IsEmpty(), TAG, "markerName is empty");

    CYISceneView *pView = YiDynamicCast<CYISceneView>(pNode);

    if (pView)
    {
        CYIAbstractTimeline *pTimeline = GetTimeline(pView, markerName);

        if (pTimeline)
        {
            if (nTimelineOffsetInms > 0)
            {
                pGroup->AddTimelineWithOffset(pTimeline, nTimelineOffsetInms);
            }
            else
            {
                pGroup->AddTimeline(pTimeline);
            }
        }
    }
}

void TimelineUtilities::RecursiveBuildSerialTimelineGroup(CYITimelineGroup *pGroup, CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms)
{
    if (pNode || pGroup)
    {
        AddNodeTimelineToGroup(pGroup, pNode, markerName, nTimelineOffsetInms);

        for (YI_UINT32 i = 0; i < pNode->GetChildCount(); i++)
        {
            RecursiveBuildNodeParallelTimelineGroup(pGroup, pNode->GetChild(i), markerName, nTimelineOffsetInms);
        }
    }
}

CYITimeline *TimelineUtilities::GetTimeline(CYISceneView *pSceneView, const CYIString &markerName)
{
    YI_ASSERT(pSceneView, TAG, "pSceneView is null");
    YI_ASSERT(!markerName.IsEmpty(), TAG, "markerName is empty");

    return pSceneView->GetTimeline(markerName);
}

CYIAnimationToggleController *TimelineUtilities::BuildDirectionAnimationController(CYISceneView *pView, const CYIString &baseName)
{
    CYIAnimationToggleController *pAnimationController = new CYIAnimationToggleController;

    if (pView && !baseName.IsEmpty())
    {
        CYIAbstractTimeline *pInTimeline = TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(pView, baseName + "In");
        pInTimeline->SetDisablesInput(true);
        CYIAbstractTimeline *pOutTimeline = TimelineUtilities::RecursiveCreateNodeParallelTimelineGroup(pView, baseName + "Out");
        pOutTimeline->SetDisablesInput(true);
        pAnimationController->SetTimelines(pInTimeline, pOutTimeline, CYIAnimationToggleController::PRIVATE);
    }
    else
    {
        YI_LOGW(TAG, "BuildDirectionAnimationController failed due to invalid view or name parameters.");
    }

    return pAnimationController;
}
CYITimeInterpolator *TimelineUtilities::GetTimeInterpolatorFromInterpolatorType(PropertyAnimationDefinition::eTimeInterpolatorType timeInterpolatorType)
{
    switch (timeInterpolatorType)
    {
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_LINEAR:
            return &CYITimeInterpolators::Linear;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_INVERT:
            return &CYITimeInterpolators::Invert;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_ELASTICEASEOUT:
            return &CYITimeInterpolators::ElasticEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_QUADEASEOUT:
            return &CYITimeInterpolators::QuadEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_QUADEASEIN:
            return &CYITimeInterpolators::QuadEaseIn;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_QUINTEASEOUT:
            return &CYITimeInterpolators::QuintEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_QUADEASEINEASEOUT:
            return &CYITimeInterpolators::QuadEaseInEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_BOUNCEEASEOUT:
            return &CYITimeInterpolators::BounceEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_BACKEASEIN:
            return &CYITimeInterpolators::BackEaseIn;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_BACKEASEOUT:
            return &CYITimeInterpolators::BackEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_SIGMOID:
            return &CYITimeInterpolators::Sigmoid;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_CIRCULAREASEIN:
            return &CYITimeInterpolators::CircularEaseIn;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_CIRCULAREASEINEASEOUT:
            return &CYITimeInterpolators::CircularEaseInEaseOut;
        case PropertyAnimationDefinition::TIME_INTERPOLATOR_TYPE_CIRCULAREASEOUT:
            return &CYITimeInterpolators::CircularEaseOut;
        default:
            return &CYITimeInterpolators::Linear;
    }
    return YI_NULL;
}

void TimelineUtilities::StartPropertyAnimation(CYISceneNode *pSceneNode, PropertyAnimationDefinition propertyAnimationDefinition)
{
    pSceneNode->StartPropertyAnimation(propertyAnimationDefinition.m_ePropType,
        propertyAnimationDefinition.m_fFromValue,
        propertyAnimationDefinition.m_fToValue,
        propertyAnimationDefinition.m_nDurationMs,
        TimelineUtilities::GetTimeInterpolatorFromInterpolatorType(propertyAnimationDefinition.m_eTimeInterpolatorType), true);
}
