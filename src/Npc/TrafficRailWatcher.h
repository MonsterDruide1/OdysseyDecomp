#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class PlacementId;
class PlacementInfo;
}  // namespace al

class TrafficRailWatcher {
public:
    struct TrafficRailActorInfo {
        const al::LiveActor* actor;
        s32 status;
    };

    TrafficRailWatcher(const al::PlacementInfo& placementInfo);

    void registerActor(const al::LiveActor* actor);
    void stopByTraffic(const al::LiveActor* actor);
    void restartByTraffic(const al::LiveActor* actor);
    bool isEqual(const al::PlacementInfo& placementInfo) const;
    bool isExist(const al::LiveActor* actor) const;
    bool tryStopByOtherNpc(const al::LiveActor* actor);
    bool tryRestartByOtherNpc(const al::LiveActor* actor);

private:
    al::PlacementId* mPlacementId;
    s32 mActorCount;
    TrafficRailActorInfo** mActors;
};

static_assert(sizeof(TrafficRailWatcher) == 0x18);
