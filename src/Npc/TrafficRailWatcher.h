#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class PlacementId;
class PlacementInfo;
PlacementId* createPlacementId(const PlacementInfo& placementInfo);
}  // namespace al

struct TrafficRailActorInfo {
    const al::LiveActor* actor;
    s32 status;
};

class TrafficRailWatcher {
public:
    TrafficRailWatcher(const al::PlacementInfo& placementInfo);

    void registerActor(const al::LiveActor* actor);
    void stopByTraffic(const al::LiveActor* actor);
    void restartByTraffic(const al::LiveActor* actor);
    bool isEqual(const al::PlacementInfo& placementInfo) const;
    bool isExist(const al::LiveActor* actor) const;
    bool tryStopByOtherNpc(const al::LiveActor* actor);
    bool tryRestartByOtherNpc(const al::LiveActor* actor);

    al::PlacementId* mPlacementId;
    s32 mActorCount;
    TrafficRailActorInfo** mActors;
};

static_assert(sizeof(TrafficRailWatcher) == 0x18);

bool isNearOnTrafficRail(const al::LiveActor* actor, const TrafficRailActorInfo* otherEntry);
