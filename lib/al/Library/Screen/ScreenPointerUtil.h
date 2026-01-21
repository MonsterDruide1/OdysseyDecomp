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

s32 compareScreenPointTarget(const ScreenPointTargetHitInfo* targetHitInfoA,
                             const ScreenPointTargetHitInfo* targetHitInfoB);
s32 compareScreenPointTargetPriorDirectPoint(const ScreenPointTargetHitInfo* targetHitInfoA,
                                             const ScreenPointTargetHitInfo* targetHitInfoB);
bool isExistScreenPointTargetKeeper(LiveActor* actor);
bool isScreenPointTargetArrayFull(LiveActor* actor);
bool isExistScreenPointTarget(LiveActor* actor, const char* name);
// TODO: Add proper parameter names for vb
ScreenPointTarget* addScreenPointTarget(LiveActor* actor, const ActorInitInfo& initInfo,
                                        const char* targetName, f32 radius, const char* jointName,
                                        const sead::Vector3f& vb);
bool hitCheckSegmentScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector3f& posStart,
                                      const sead::Vector3f& posEnd);
bool hitCheckScreenCircleScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector2f& pos,
                                           f32 radius, f32 screenRadius);
bool hitCheckLayoutCircleScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector2f& pos,
                                           f32 radius, f32 layoutRadius,
                                           s32 (*cmp)(const ScreenPointTargetHitInfo*,
                                                      const ScreenPointTargetHitInfo*));
bool isHitScreenPointTarget(ScreenPointer* screenPointer, const ScreenPointTarget* target);
bool sendMsgScreenPointTarget(const SensorMsg& message, ScreenPointer* screenPointer,
                              ScreenPointTarget* target);
s32 getHitTargetNum(ScreenPointer* screenPointer);
const sead::Vector3f& getHitTargetPos(ScreenPointer* screenPointer, s32 index);
f32 getHitTargetRadius(ScreenPointer* screenPointer, s32 index);
}  // namespace al

namespace alScreenPointFunction {
void updateScreenPointAll(al::LiveActor* actor);
}  // namespace alScreenPointFunction
