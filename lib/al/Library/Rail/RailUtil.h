#pragma once

#include <math/seadVector.h>

namespace al {
class IUseRail;
class LiveActor;

void setRailPosToStart(IUseRail* railHolder);
void moveSyncRail(LiveActor* actor, f32 speed);
void moveSyncRailLoop(LiveActor* actor, f32 speed);
void moveSyncRailTurn(LiveActor* actor, f32 speed);
void calcRailUp(sead::Vector3f* out, IUseRail* railHolder);
const sead::Vector3f& getRailDir(IUseRail* railHolder);
bool isExistRail(IUseRail* railHolder);
void setRailClippingInfo(sead::Vector3f*, LiveActor* actor, f32, f32);
}  // namespace al
