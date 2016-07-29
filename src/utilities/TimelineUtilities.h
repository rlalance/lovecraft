#ifndef _TIMELINEUTILITIES_H
#define _TIMELINEUTILITIES_H

#include <animation/YiTimelineGroup.h>
#include <utility/YiString.h>
#include <scenetree/YiSceneNode.h>

class CYIAbstractTimeline;
class CYIAnimationToggleController;
class CYISceneNode;
class CYISceneView;
class CYITimeline;

/*!
 *  Definition for a property animation that will eventually be used to play an animation on a CYISceneNode
 * \details
 */
class PropertyAnimationDefinition
{
public:
    enum eTimeInterpolatorType
    {
        TIME_INTERPOLATOR_TYPE_LINEAR,
        TIME_INTERPOLATOR_TYPE_INVERT,
        TIME_INTERPOLATOR_TYPE_ELASTICEASEOUT,
        TIME_INTERPOLATOR_TYPE_QUADEASEOUT,
        TIME_INTERPOLATOR_TYPE_QUADEASEIN,
        TIME_INTERPOLATOR_TYPE_QUINTEASEOUT,
        TIME_INTERPOLATOR_TYPE_QUADEASEINEASEOUT,
        TIME_INTERPOLATOR_TYPE_BOUNCEEASEOUT,
        TIME_INTERPOLATOR_TYPE_BACKEASEIN,
        TIME_INTERPOLATOR_TYPE_BACKEASEOUT,
        TIME_INTERPOLATOR_TYPE_SIGMOID,
        TIME_INTERPOLATOR_TYPE_CIRCULAREASEIN,
        TIME_INTERPOLATOR_TYPE_CIRCULAREASEINEASEOUT,
        TIME_INTERPOLATOR_TYPE_CIRCULAREASEOUT
    };

    PropertyAnimationDefinition();
    PropertyAnimationDefinition(YI_NODE_PROPERTY_TYPE propType, YI_FLOAT fFrom, YI_FLOAT fTo, YI_UINT32 nDurMs, eTimeInterpolatorType timeInterpolatorType);

    YI_NODE_PROPERTY_TYPE m_ePropType;
    YI_FLOAT m_fFromValue;
    YI_FLOAT m_fToValue;
    YI_UINT32 m_nDurationMs;
    eTimeInterpolatorType m_eTimeInterpolatorType;
    static YI_UINT32 DEFAULT_PROPERTY_ANIMATION_DURATION_IN_MILLISECONDS;
};

/*
 * Unless otherwise specified, TimelineUtilities will leave the ownership of timelines to the CYISceneView by using GetTimeline methods.
 * If a method returns timeline group, the caller is not responsible for deleting timelines in the group.
 */
namespace TimelineUtilities
{
    /*!
     *  \details Returns one of the engine built in interpolators.
     */
    CYITimeInterpolator *GetTimeInterpolatorFromInterpolatorType(PropertyAnimationDefinition::eTimeInterpolatorType type);
    CYITimelineGroup *RecursiveCreateNodeParallelTimelineGroup(CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms = 0);
    CYITimelineGroup *RecursiveCreateNodeSerialTimelineGroup(CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms = 0);
    void RecursiveBuildNodeParallelTimelineGroup(CYITimelineGroup *pGroup, CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms = 0);
    void RecursiveBuildSerialTimelineGroup(CYITimelineGroup *pGroup, CYISceneNode *pNode, CYIString markerName, YI_UINT64 nTimelineOffsetInms = 0);
    CYITimeline *GetTimeline(CYISceneView *pSceneView, const CYIString &markerName);
    void AddNodeTimelineToGroup(CYITimelineGroup *pGroup, const CYISceneNode *pNode, const CYIString &markerName, YI_UINT64 nTimelineOffsetInms = 0);
    CYIAnimationToggleController *BuildDirectionAnimationController(CYISceneView *pView, const CYIString &baseName);
    void StartPropertyAnimation(CYISceneNode *pSceneNode, PropertyAnimationDefinition propertyAnimationDefinition);
    bool HasViewFocusInTimelines(CYISceneView *pView);
};

#endif
