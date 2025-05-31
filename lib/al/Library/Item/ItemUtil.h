#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class ActorItemInfo;
class HitSensor;
class LiveActor;

bool isExistItemKeeper(const LiveActor* actor);
ActorItemInfo* addItem(LiveActor* actor, const ActorInitInfo&, const char*, const char*,
                       const char*, s32, bool);
ActorItemInfo* addItem(LiveActor* actor, const ActorInitInfo&, const char*, bool);
void setAppearItemFactor(const LiveActor* actor, const char*, const HitSensor* sensor);
void setAppearItemOffset(const LiveActor* actor, const sead::Vector3f& offset);
void setAppearItemAttackerSensor(const LiveActor* actor, const HitSensor* sensor);
bool appearItem(const LiveActor* actor);
bool appearItem(const LiveActor* actor, const sead::Vector3f&, const sead::Quatf&,
                const HitSensor* sensor);
bool appearItem(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                const HitSensor* sensor);
bool appearItemTiming(const LiveActor* actor, const char*);
bool appearItemTiming(const LiveActor* actor, const char*, const sead::Vector3f&,
                      const sead::Quatf&, const HitSensor* sensor);
bool appearItemTiming(const LiveActor* actor, const char*, const sead::Vector3f&,
                      const sead::Vector3f&, const HitSensor* sensor);
void acquireItem(const LiveActor* actor, HitSensor* sensor, const char*);
s32 getItemType(const LiveActor* actor, const char*);
}  // namespace al
