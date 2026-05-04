#include "Npc/TrafficRailWatcher.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Rail/RailUtil.h"

TrafficRailWatcher::TrafficRailWatcher(const al::PlacementInfo& placementInfo) {
    mPlacementId = al::createPlacementId(placementInfo);
    mActors = new ActorInfo*[32];
    for (s32 i = 0; i < 32; i++)
        mActors[i] = nullptr;
}

void TrafficRailWatcher::registerActor(const al::LiveActor* actor) {
    mActors[mActorCount] = new ActorInfo(actor);
    mActorCount++;
}

inline TrafficRailWatcher::ActorInfo* getActor(TrafficRailWatcher::ActorInfo** actorList,
                                               const al::LiveActor* actor) {
    // BUG: No bounds checking. Requesting an actor that is not in the actor list will softlock the
    // thread
    for (s32 i = 0; true; i++) {
        TrafficRailWatcher::ActorInfo* info = actorList[i];
        if (info->actor == actor)
            return info;
    }
    return nullptr;
}

void TrafficRailWatcher::stopByTraffic(const al::LiveActor* actor) {
    getActor(mActors, actor)->status = ActorStatus::StoppedByTraffic;
}

void TrafficRailWatcher::restartByTraffic(const al::LiveActor* actor) {
    getActor(mActors, actor)->status = ActorStatus::Normal;
}

bool TrafficRailWatcher::isEqual(const al::PlacementInfo& placementInfo) const {
    al::PlacementId placementId;
    al::getPlacementId(&placementId, placementInfo);
    return al::isEqualPlacementId(*mPlacementId, placementId);
}

bool TrafficRailWatcher::isExist(const al::LiveActor* actor) const {
    for (s32 i = 0; i < mActorCount; i++)
        if (mActors[i]->actor == actor)
            return true;
    return false;
}

// TODO: Add to sead as a math util
inline f32 modLength(f32 value, f32 length) {
    return al::modf(value + length, length) + 0.0f;
}

bool isNearOnTrafficRail(const al::LiveActor* actor,
                         const TrafficRailWatcher::ActorInfo* otherInfo) {
    if (otherInfo->status != TrafficRailWatcher::ActorStatus::StoppedByTraffic &&
        otherInfo->status != TrafficRailWatcher::ActorStatus::StoppedByNpc)
        return false;

    const al::LiveActor* other = otherInfo->actor;

    if (!al::isRailGoingToEnd(actor) && al::isRailGoingToEnd(other))
        return false;

    f32 actorCoord = al::getRailCoord(actor);
    f32 nextRailCoord = modLength(al::getRailCoord(actor) + 150.0f, al::getRailTotalLength(actor));

    f32 startCoord = actorCoord;
    f32 endCoord = nextRailCoord;
    if (!al::isRailGoingToEnd(actor)) {
        // We are going backwards, revert direction
        endCoord = actorCoord;
        startCoord = nextRailCoord;
    }

    if (al::isLoopRail(actor) && endCoord < startCoord) {
        if (startCoord < al::getRailCoord(other))
            return true;
        return al::getRailCoord(other) < endCoord;
    }

    if (startCoord < al::getRailCoord(other))
        return al::getRailCoord(other) < endCoord;
    return false;

    return al::getRailCoord(other) < endCoord;
}

bool TrafficRailWatcher::tryStopByOtherNpc(const al::LiveActor* actor) {
    ActorInfo* actorInfo = nullptr;
    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i]->actor == actor) {
            actorInfo = mActors[i];
            break;
        }
    }

    for (s32 j = 0; j < mActorCount; j++) {
        if (mActors[j] != actorInfo && isNearOnTrafficRail(actor, mActors[j])) {
            actorInfo->status = ActorStatus::StoppedByNpc;
            return true;
        }
    }
    return false;
}

bool TrafficRailWatcher::tryRestartByOtherNpc(const al::LiveActor* actor) {
    ActorInfo* actorInfo = nullptr;
    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i]->actor == actor) {
            actorInfo = mActors[i];
            break;
        }
    }

    for (s32 i = 0; i < mActorCount; i++)
        if (mActors[i] != actorInfo && isNearOnTrafficRail(actor, mActors[i]))
            return false;

    actorInfo->status = ActorStatus::Normal;
    return true;
}
