#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class PlacementId;
class PlacementInfo;
}  // namespace al

class TrafficRailWatcher {
public:
    enum class ActorStatus { Normal, StoppedByTraffic, StoppedByNpc };

    struct ActorInfo {
        ActorInfo(const al::LiveActor* liveActor) {
            actor = liveActor;
            status = ActorStatus::Normal;
        }

        const al::LiveActor* actor;
        ActorStatus status;
    };

    TrafficRailWatcher(const al::PlacementInfo& placementInfo);

    bool isEqual(const al::PlacementInfo& placementInfo) const;
    void registerActor(const al::LiveActor* actor);
    bool isExist(const al::LiveActor* actor) const;
    void stopByTraffic(const al::LiveActor* actor);
    void restartByTraffic(const al::LiveActor* actor);
    bool tryStopByOtherNpc(const al::LiveActor* actor);
    bool tryRestartByOtherNpc(const al::LiveActor* actor);

private:
    al::PlacementId* mPlacementId = nullptr;
    s32 mActorCount = 0;
    ActorInfo** mActors = nullptr;
};

static_assert(sizeof(TrafficRailWatcher) == 0x18);
