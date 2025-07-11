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

class RailPlacementCallBack {
public:
    virtual void call(const sead::Vector3f& pos, s32 index, f32 coord) const;
};

void setRailPosToStart(IUseRail* railHolder);
void setRailPosToEnd(IUseRail* railHolder);
void setRailPosToNearestPos(IUseRail* railHolder, const sead::Vector3f& pos);
void setRailPosToCoord(IUseRail* railHolder, f32 coord);
void setRailPosToRailPoint(IUseRail* railHolder, s32 index);
void setSyncRailToStart(LiveActor* actor);
void syncRailTrans(LiveActor* actor);
void setSyncRailToEnd(LiveActor* actor);
void setSyncRailToNearestPos(LiveActor* actor, const sead::Vector3f& pos);
void setSyncRailToNearestRailControlPoint(LiveActor* actor);
void setSyncRailToNearestPos(LiveActor* actor);
void setSyncRailToCoord(LiveActor* actor, f32 coord);
void setSyncRailToRailPoint(LiveActor* actor, s32 index);
bool moveRail(IUseRail* railHolder, f32 speed);
bool isRailReachedGoal(const IUseRail* railHolder);
bool moveRailLoop(IUseRail* railHolder, f32 speed);
f32 getRailCoord(const IUseRail* railHolder);
bool isRailGoingToEnd(const IUseRail* railHolder);
f32 getRailTotalLength(const IUseRail* railHolder);
bool moveRailTurn(IUseRail* railHolder, f32 speed, f32 goalCoord);
void reverseRail(IUseRail* railHolder);
bool isRailReachedNearGoal(const IUseRail* railHolder, f32 goalCoord);
bool turnToRailDir(LiveActor* actor, f32 deg);
void calcRailMoveDir(sead::Vector3f* moveDir, const IUseRail* railHolder);
bool turnToRailDirImmediately(LiveActor* actor);
const sead::Vector3f& getRailPos(const IUseRail* railHolder);
bool moveSyncRail(LiveActor* actor, f32 speed);
bool moveSyncRailLoop(LiveActor* actor, f32 speed);
bool moveSyncRailTurn(LiveActor* actor, f32 speed);
f32 calcNearestRailCoord(const IUseRail* railHolder, const sead::Vector3f& pos);
f32 calcNearestRailPos(sead::Vector3f* railPos, const IUseRail* railHolder,
                       const sead::Vector3f& pos);
void calcNearestRailDir(sead::Vector3f* dir, const IUseRail* railHolder, const sead::Vector3f& pos);
void calcNearestRailPosAndDir(sead::Vector3f* pos, sead::Vector3f* dir, const IUseRail* railHolder,
                              const sead::Vector3f& interval);
void calcRailPosDir(sead::Vector3f* railPos, sead::Vector3f* dir, const IUseRail* railHolder,
                    f32 coord);
void calcRailPointPos(sead::Vector3f* pos, const IUseRail* railHolder, s32 index);
s32 calcNearestRailPointNo(const IUseRail* railHolder, const sead::Vector3f&);
s32 calcNearestRailPointNo(const IUseRail* railHolder);
s32 calcCoordNearestRailPointNo(const IUseRail* railHolder);
s32 getRailPartIndex(const IUseRail* railHolder);
f32 calcRailPartRate(const IUseRail* railHolder);
bool isLoopRail(const IUseRail* railHolder);
s32 getRailPointNum(const IUseRail* railHolder);
f32 calcNearestRailPointPosCoord(const IUseRail* railHolder, const sead::Vector3f& pos);
f32 calcRailCoordByPoint(const IUseRail* railHolder, s32 index);
void calcRailUp(sead::Vector3f* pos, const IUseRail* railHolder);
PlacementInfo* getRailPointInfo(const IUseRail* railHolder, s32 index);
s32 getRailPointNo(const IUseRail* railHolder);
s32 getNextRailPointNo(const IUseRail* railHolder);
f32 calcRailToGoalLength(const IUseRail* railHolder);
f32 calcRailTotalRate(const IUseRail* railHolder);
f32 calcRailToNextRailPointLength(const IUseRail* railHolder);
f32 calcRailToPreviousRailPointLength(const IUseRail* railHolder);
s32 getRailNum(const IUseRail* railHolder);
const sead::Vector3f& getRailDir(const IUseRail* railHolder);
bool isRailReachedEnd(const IUseRail* railHolder);
f32 getRailPartLength(const IUseRail* railHolder, s32 index);
bool tryGetCurrentRailPointArg(f32* arg, const IUseRail* railHolder, const char* argName);
bool tryGetNextRailPointArg(f32* arg, const IUseRail* railHolder, const char* argName);
bool tryGetCurrentRailPointArg(s32* arg, const IUseRail* railHolder, const char* argName);
bool tryGetNextRailPointArg(s32* arg, const IUseRail* railHolder, const char* argName);
bool tryGetCurrentRailPointArg(bool* arg, const IUseRail* railHolder, const char* argName);
bool tryGetNextRailPointArg(bool* arg, const IUseRail* railHolder, const char* argName);
bool tryGetCurrentRailPointArg(const char** arg, const IUseRail* railHolder, const char* argName);
bool tryGetNextRailPointArg(const char** arg, const IUseRail* railHolder, const char* argName);
bool isExistRail(const IUseRail* railHolder);
bool isRailReachedStart(const IUseRail* railHolder);
bool isRailReachedNearGoal(const IUseRail* railHolder, f32 goalCoordStart, f32 goalCoordEnd);
bool isRailReachedEdge(const IUseRail* railHolder);
bool isRailReachedNearRailPoint(const IUseRail* railHolder, f32 epsilon);
bool isRailIncludeBezierPart(const IUseRail* railHolder);
bool isRailBezierPart(const IUseRail* railHolder, s32 index);
bool isRailPlusDir(const IUseRail* railHolder, const sead::Vector3f& dir);
bool isRailPlusPoseSide(const LiveActor* actor);
bool isRailPlusPoseUp(const LiveActor* actor);
bool isRailPlusPoseFront(const LiveActor* actor);
void calcRailPosAtCoord(sead::Vector3f* dir, const IUseRail* railHolder, f32 coord);
void calcRailDirAtCoord(sead::Vector3f* dir, const IUseRail* railHolder, f32 coord);
void calcRailPosFront(sead::Vector3f* pos, const IUseRail* railHolder, f32 offset);
void calcRailDirFront(sead::Vector3f* pos, const IUseRail* railHolder, f32 offset);
void calcRailPointPose(sead::Quatf* pose, const IUseRail* railHolder, s32 index);
s32 calcRailPointNum(const IUseRail* railHolder, f32 distance1, f32 distance2);
s32 calcForwardRailPointNo(const IUseRail* railHolder, s32 index);
s32 calcPreviousRailPointNo(const IUseRail* railHolder, s32 index);
void calcRailClippingInfo(sead::Vector3f* pos, f32* distance, const IUseRail* railHolder, f32 step,
                          f32 offset);
void setRailClippingInfo(sead::Vector3f* pos, LiveActor* actor, f32 step, f32 offset);
void calcAndSyncMoveFitRailDir(IUseRail* railHolder, sead::Vector3f* out, f32 step,
                               const sead::Vector3f& pos, const sead::Vector3f& vertical,
                               bool reverse);
void calcAndSyncMoveFitRailDir(IUseRail* railHolder, sead::Vector3f* out, f32 step,
                               const sead::Vector3f& pos, const sead::Vector3f& vertical,
                               bool reverse);
LiveActorGroup* createRailModelGroup(const ActorInitInfo& info, const sead::Vector3f& prevPointPos,
                                     const sead::Vector3f& pointPos, const char* actorName,
                                     const char* archiveName);
LiveActor* createRailModelActor(const ActorInitInfo& info, const sead::Vector3f& prevPointPos,
                                const sead::Vector3f& pointPos, const char* actorName,
                                const char* archiveName);
s32 calcRailDivideNum(const IUseRail* railHolder, f32 segmentLength, bool isForce);
s32 calcRailDivideForcePointNum(const IUseRail* railHolder, f32 segmentLength);
s32 calcRailDivideEquallyNum(const IUseRail* railHolder, f32 segmentLength);
void placementRailDivide(const IUseRail* railHolder, f32 segmentLength, bool isForce,
                         const RailPlacementCallBack& callback);
void placementRailDivideForcePoint(const IUseRail* railHolder, f32 segmentLength,
                                   const RailPlacementCallBack& callback);
void placementRailDivideEqually(const IUseRail* railHolder, f32 segmentLength,
                                const RailPlacementCallBack& callback);
}  // namespace al
