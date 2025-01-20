#pragma once

#include <basis/seadTypes.h>

namespace al {
class ActorInitInfo;
class LiveActor;
class HitSensor;
class PlacementId;

class SupportFreezeSyncGroup {
public:
    SupportFreezeSyncGroup();

    void init(const ActorInitInfo& info);
    /**
     * @warning This function doesn't check if mActorCount is greater than mMaxActorCount.
     *          Possible buffer overflow.
     */
    void regist(LiveActor* actor);
    void setHostSensor(HitSensor* hostSensor);
    bool isEqualGroupId(const ActorInitInfo& info) const;
    void movement();

private:
    PlacementId* mGroupId;
    HitSensor* mHostSensor = nullptr;
    LiveActor** mActors = nullptr;
    s32 mActorCount = 0;
    s32 mMaxActorCount = 64;
};
}  // namespace al
