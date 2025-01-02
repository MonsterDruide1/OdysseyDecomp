#pragma once

#include <math/seadVector.h>

namespace al {
class IUseRail;
class LiveActor;

void setSyncRailToNearestPos(LiveActor* actor);
void setSyncRailToCoord(LiveActor* actor, f32 coord);
void setRailPosToStart(IUseRail* railHolder);
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
}  // namespace al
