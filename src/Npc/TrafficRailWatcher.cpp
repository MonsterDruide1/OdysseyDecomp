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
    auto* info = new ActorInfo;
    info->actor = actor;
    info->status = ActorStatus::Normal;
    mActors[mActorCount] = info;
    mActorCount++;
}

void TrafficRailWatcher::stopByTraffic(const al::LiveActor* actor) {
    ActorInfo** p = mActors;
    ActorInfo* info;
    while (info = *p, info->actor != actor)
        info = *p++;
    info->status = ActorStatus::StoppedByTraffic;
}

void TrafficRailWatcher::restartByTraffic(const al::LiveActor* actor) {
    ActorInfo** p = mActors;
    ActorInfo* info;
    while (info = *p, info->actor != actor)
        info = *p++;
    info->status = ActorStatus::Normal;
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

bool isNearOnTrafficRail(const al::LiveActor* actor,
                         const TrafficRailWatcher::ActorInfo* otherInfo) {
    if (otherInfo->status != TrafficRailWatcher::ActorStatus::StoppedByTraffic &&
        otherInfo->status != TrafficRailWatcher::ActorStatus::StoppedByNpc)
        return false;

    const al::LiveActor* other = otherInfo->actor;

    if (!al::isRailGoingToEnd(actor) && al::isRailGoingToEnd(other))
        return false;

    f32 actorCoord = al::getRailCoord(actor);
    f32 nearBound = al::getRailCoord(actor) + 150.0f;
    f32 totalLength = al::getRailTotalLength(actor);
    f32 wrapped = al::modf(nearBound + totalLength, totalLength) + 0.0f;

    f32 low, high;
    if (al::isRailGoingToEnd(actor)) {
        low = actorCoord;
        high = wrapped;
    } else {
        high = actorCoord;
        low = wrapped;
    }

    if (al::isLoopRail(actor) && high < low) {
        if (low < al::getRailCoord(other))
            return true;
    } else {
        if (!(low < al::getRailCoord(other)))
            return false;
    }

    return al::getRailCoord(other) < high;
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
