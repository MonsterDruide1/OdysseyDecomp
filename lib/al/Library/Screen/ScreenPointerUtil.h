#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
class ScreenPointer;
class ScreenPointTarget;
struct ScreenPointTargetHitInfo;
class SensorMsg;

s32 compareScreenPointTarget(const ScreenPointTargetHitInfo*, const ScreenPointTargetHitInfo*);
s32 compareScreenPointTargetPriorDirectPoint(const ScreenPointTargetHitInfo*,
                                             const ScreenPointTargetHitInfo*);
bool isExistScreenPointTargetKeeper(LiveActor*);
bool isScreenPointTargetArrayFull(LiveActor*);
bool isExistScreenPointTarget(LiveActor*, const char*);
ScreenPointTarget* addScreenPointTarget(LiveActor*, const ActorInitInfo&, const char*, f32,
                                        const char*, const sead::Vector3f&);
bool hitCheckSegmentScreenPointTarget(ScreenPointer*, const sead::Vector3f&, const sead::Vector3f&);
bool hitCheckScreenCircleScreenPointTarget(ScreenPointer*, const sead::Vector2f&, f32, f32);
bool hitCheckLayoutCircleScreenPointTarget(ScreenPointer*, const sead::Vector2f&, f32, f32,
                                           s32 (*)(const ScreenPointTargetHitInfo*,
                                                   const ScreenPointTargetHitInfo*));
bool isHitScreenPointTarget(ScreenPointer*, const ScreenPointTarget*);
bool sendMsgScreenPointTarget(const SensorMsg&, ScreenPointer*, ScreenPointTarget*);
s32 getHitTargetNum(ScreenPointer*);
const sead::Vector3f& getHitTargetPos(ScreenPointer*, s32);
f32 getHitTargetRadius(ScreenPointer*, s32);
}  // namespace al

namespace alScreenPointFunction {
void updateScreenPointAll(al::LiveActor* actor);
}  // namespace alScreenPointFunction
