#pragma once

#include "Library/Placement/PlacementId.h"

namespace al {
class HitSensor;
class LiveActor;
struct ActorInitInfo;

class SupportFreezeSyncGroup {
public:
    SupportFreezeSyncGroup();

    void init(const ActorInitInfo& info);
    void regist(LiveActor* actor);
    void setHostSensor(HitSensor* hostSensor);
    bool isEqualGroupId(const ActorInitInfo& info) const;
    void movement();

private:
    PlacementId* mSupportFreezeSyncGroupId = new PlacementId{};
    HitSensor* mHostSensor = nullptr;
    LiveActor** mActors = nullptr;
    s32 mActorCount = 0;
    s32 mMaxActorCount = 64;
};
}  // namespace al
