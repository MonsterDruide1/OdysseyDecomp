#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class HitSensor;
class LiveActor;

bool isExistItemKeeper(const LiveActor* actor);
void addItem(LiveActor* actor, const ActorInitInfo&, const char*, const char*, const char*, s32,
             bool);
void addItem(LiveActor* actor, const ActorInitInfo&, const char*, bool);
void setAppearItemFactor(const LiveActor* actor, const char*, const HitSensor* sensor);
void setAppearItemOffset(const LiveActor* actor, const sead::Vector3f& offset);
void setAppearItemAttackerSensor(const LiveActor* actor, const HitSensor* sensor);
void appearItem(const LiveActor* actor);
void appearItem(const LiveActor* actor, const sead::Vector3f&, const sead::Quatf&,
                const HitSensor* sensor);
void appearItem(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                const HitSensor* sensor);
void appearItemTiming(const LiveActor* actor, const char*);
void appearItemTiming(const LiveActor* actor, const char*, const sead::Vector3f&,
                      const sead::Quatf&, const HitSensor* sensor);
void appearItemTiming(const LiveActor* actor, const char*, const sead::Vector3f&,
                      const sead::Vector3f&, const HitSensor* sensor);
void acquireItem(const LiveActor* actor, HitSensor* sensor, const char*);
s32 getItemType(const LiveActor* actor, const char*);
}  // namespace al
