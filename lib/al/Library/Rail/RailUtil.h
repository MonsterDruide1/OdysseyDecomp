#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class IUseRail;
class LiveActor;
class LiveActorGroup;
class PlacementInfo;
class RailPlacementCallBack;

void setRailPosToStart(IUseRail* railHolder);
void setRailPosToEnd(IUseRail* railHolder);
void setRailPosToNearestPos(IUseRail* railHolder, const sead::Vector3f&);
void setRailPosToCoord(IUseRail* railHolder, f32);
void setRailPosToRailPoint(IUseRail* railHolder, s32);
void setSyncRailToStart(LiveActor* actor);
void syncRailTrans(LiveActor* actor);
void setSyncRailToEnd(LiveActor* actor);
void setSyncRailToNearestPos(LiveActor* actor, const sead::Vector3f&);
void setSyncRailToNearestRailControlPoint(LiveActor* actor);
void setSyncRailToNearestPos(LiveActor* actor);
void setSyncRailToCoord(LiveActor* actor, f32 coord);
void setSyncRailToRailPoint(LiveActor* actor, s32);
bool moveRail(IUseRail* railHolder, f32);
bool isRailReachedGoal(const IUseRail* railHolder);
bool moveRailLoop(IUseRail* railHolder, f32);
f32 getRailCoord(const IUseRail* railHolder);
bool isRailGoingToEnd(const IUseRail* railHolder);
f32 getRailTotalLength(const IUseRail* railHolder);
bool moveRailTurn(IUseRail* railHolder, f32, f32);
void reverseRail(IUseRail* railHolder);
bool isRailReachedNearGoal(const IUseRail* railHolder, f32);
bool turnToRailDir(LiveActor* actor, f32);
void calcRailMoveDir(sead::Vector3f*, const IUseRail* railHolder);
bool turnToRailDirImmediately(LiveActor* actor);
const sead::Vector3f& getRailPos(const IUseRail* railHolder);
bool moveSyncRail(LiveActor* actor, f32);
bool moveSyncRailLoop(LiveActor* actor, f32 speed);
bool moveSyncRailTurn(LiveActor* actor, f32 speed);
f32 calcNearestRailCoord(const IUseRail* railHolder, const sead::Vector3f&);
f32 calcNearestRailPos(sead::Vector3f*, const IUseRail* railHolder, const sead::Vector3f&);
void calcNearestRailDir(sead::Vector3f*, const IUseRail* railHolder, const sead::Vector3f&);
void calcNearestRailPosAndDir(sead::Vector3f*, sead::Vector3f*, const IUseRail* railHolder,
                              const sead::Vector3f&);
void calcRailPosDir(sead::Vector3f*, sead::Vector3f*, const IUseRail* railHolder, f32);
void calcRailPointPos(sead::Vector3f*, const IUseRail* railHolder, s32);
s32 calcNearestRailPointNo(const IUseRail* railHolder, const sead::Vector3f&);
s32 calcNearestRailPointNo(const IUseRail* railHolder);
s32 calcCoordNearestRailPointNo(const IUseRail* railHolder);
s32 getRailPartIndex(const IUseRail* railHolder);
f32 calcRailPartRate(const IUseRail* railHolder);
bool isLoopRail(const IUseRail* railHolder);
s32 getRailPointNum(const IUseRail* railHolder);
f32 calcNearestRailPointPosCoord(const IUseRail* railHolder, const sead::Vector3f&);
f32 calcRailCoordByPoint(const IUseRail* railHolder, s32);
void calcRailUp(sead::Vector3f*, const IUseRail* railHolder);
PlacementInfo* getRailPointInfo(const IUseRail* railHolder, s32);
s32 getRailPointNo(const IUseRail* railHolder);
s32 getNextRailPointNo(const IUseRail* railHolder);
f32 calcRailToGoalLength(const IUseRail* railHolder);
f32 calcRailTotalRate(const IUseRail* railHolder);
f32 calcRailToNextRailPointLength(const IUseRail* railHolder);
f32 calcRailToPreviousRailPointLength(const IUseRail* railHolder);
s32 getRailNum(const IUseRail* railHolder);
const sead::Vector3f& getRailDir(const IUseRail* railHolder);
bool isRailReachedEnd(const IUseRail* railHolder);
f32 getRailPartLength(const IUseRail* railHolder, s32);
bool tryGetCurrentRailPointArg(f32*, const IUseRail* railHolder, const char*);
bool tryGetNextRailPointArg(f32*, const IUseRail* railHolder, const char*);
bool tryGetCurrentRailPointArg(s32*, const IUseRail* railHolder, const char*);
bool tryGetNextRailPointArg(s32*, const IUseRail* railHolder, const char*);
bool tryGetCurrentRailPointArg(bool*, const IUseRail* railHolder, const char*);
bool tryGetNextRailPointArg(bool*, const IUseRail* railHolder, const char*);
bool tryGetCurrentRailPointArg(const char**, const IUseRail* railHolder, const char*);
bool tryGetNextRailPointArg(const char**, const IUseRail* railHolder, const char*);
bool isExistRail(const IUseRail* railHolder);
bool isRailReachedStart(const IUseRail* railHolder);
bool isRailReachedNearGoal(const IUseRail* railHolder, f32, f32);
bool isRailReachedEdge(const IUseRail* railHolder);
bool isRailReachedNearRailPoint(const IUseRail* railHolder, f32);
bool isRailIncludeBezierPart(const IUseRail* railHolder);
bool isRailBezierPart(const IUseRail* railHolder, s32);
bool isRailPlusDir(const IUseRail* railHolder, const sead::Vector3f&);
bool isRailPlusPoseSide(const LiveActor* actor);
bool isRailPlusPoseUp(const LiveActor* actor);
bool isRailPlusPoseFront(const LiveActor* actor);
void calcRailPosAtCoord(sead::Vector3f*, const IUseRail* railHolder, f32);
void calcRailDirAtCoord(sead::Vector3f*, const IUseRail* railHolder, f32);
void calcRailPosFront(sead::Vector3f*, const IUseRail* railHolder, f32);
void calcRailDirFront(sead::Vector3f*, const IUseRail* railHolder, f32);
void calcRailPointPose(sead::Quatf*, const IUseRail* railHolder, s32);
s32 calcRailPointNum(const IUseRail* railHolder, f32, f32);
s32 calcForwardRailPointNo(const IUseRail* railHolder, s32);
s32 calcPreviousRailPointNo(const IUseRail* railHolder, s32);
void calcRailClippingInfo(sead::Vector3f*, f32*, const IUseRail* railHolder, f32, f32);
void setRailClippingInfo(sead::Vector3f*, LiveActor* actor, f32, f32);
void calcAndSyncMoveFitRailDir(IUseRail* railHolder, sead::Vector3f*, f32, const sead::Vector3f&,
                               const sead::Vector3f&, bool);
void calcAndSyncMoveFitRailDir(LiveActor* actor, sead::Vector3f*, f32, bool);
LiveActorGroup* createRailModelGroup(const IUseRail* railHolder, const ActorInitInfo&, const char*,
                                     const char*);
s32 calcRailDivideNum(const IUseRail* railHolder, f32, bool);
s32 calcRailDivideForcePointNum(const IUseRail* railHolder, f32);
s32 calcRailDivideEquallyNum(const IUseRail* railHolder, f32);
void placementRailDivide(const IUseRail* railHolder, f32, bool, const RailPlacementCallBack&);
void placementRailDivideForcePoint(const IUseRail* railHolder, f32, const RailPlacementCallBack&);
void placementRailDivideEqually(const IUseRail* railHolder, f32, const RailPlacementCallBack&);
}  // namespace al
