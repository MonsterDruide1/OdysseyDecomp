#pragma once

#include <math/seadVector.h>

namespace al {
class IUseRail;
class LiveActor;
class PlacementInfo;

void setSyncRailToNearestPos(LiveActor* actor);
void setSyncRailToCoord(LiveActor* actor, f32 coord);
void setRailPosToStart(IUseRail* railHolder);
void calcRailMoveDir(sead::Vector3f* out, const IUseRail* railHolder);
void moveSyncRail(LiveActor* actor, f32 speed);
void moveSyncRailLoop(LiveActor* actor, f32 speed);
void moveSyncRailTurn(LiveActor* actor, f32 speed);
void calcRailUp(sead::Vector3f* out, const IUseRail* railHolder);
const sead::Vector3f& getRailDir(const IUseRail* railHolder);
f32 getRailCoord(const IUseRail* railHolder);
bool isExistRail(const IUseRail* railHolder);
void setRailClippingInfo(sead::Vector3f*, LiveActor* actor, f32, f32);
bool isRailReachedGoal(const IUseRail* railHolder);
bool isRailReachedEnd(const IUseRail* railHolder);
PlacementInfo* getRailPointInfo(const IUseRail* railHolder, s32 index);
void calcRailPointPos(sead::Vector3f* out, const IUseRail* railHolder, s32 index);
s32 getRailPointNum(const IUseRail* railHolder);
bool isRailPlusDir(const IUseRail* railHolder, const sead::Vector3f& dir);
f32 getRailTotalLength(const IUseRail* railHolder);
bool turnToRailDir(LiveActor* actor, f32 speed);
}  // namespace al
