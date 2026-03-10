#include "Npc/TrafficRailWatcher.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Rail/RailUtil.h"

TrafficRailWatcher::TrafficRailWatcher(const al::PlacementInfo& placementInfo)
    : mPlacementId(nullptr), mActorCount(0), mActors(nullptr) {
    mPlacementId = al::createPlacementId(placementInfo);
    mActors = new TrafficRailActorInfo*[32];
    for (s32 i = 0; i < 32; i++)
        mActors[i] = nullptr;
}

void TrafficRailWatcher::registerActor(const al::LiveActor* actor) {
    auto* info = new TrafficRailActorInfo;
    info->actor = actor;
    info->status = 0;
    mActors[mActorCount] = info;
    mActorCount++;
}

void TrafficRailWatcher::stopByTraffic(const al::LiveActor* actor) {
    TrafficRailActorInfo** p = mActors;
    TrafficRailActorInfo* info;
    while (info = *p, info->actor != actor)
        info = *p++;
    info->status = 1;
}

void TrafficRailWatcher::restartByTraffic(const al::LiveActor* actor) {
    TrafficRailActorInfo** p = mActors;
    TrafficRailActorInfo* info;
    while (info = *p, info->actor != actor)
        info = *p++;
    info->status = 0;
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

// NON_MATCHING
bool isNearOnTrafficRail(const al::LiveActor* actor,
                         const TrafficRailWatcher::TrafficRailActorInfo* otherEntry) {
    if (otherEntry->status != 1 && otherEntry->status != 2)
        return false;

    const al::LiveActor* otherActor = otherEntry->actor;

    if (!al::isRailGoingToEnd(actor) && al::isRailGoingToEnd(otherActor))
        return false;

    f32 actorCoord = al::getRailCoord(actor);
    f32 nearBound = al::getRailCoord(actor) + 150.0f;
    f32 totalLength = al::getRailTotalLength(actor);
    f32 wrapped = al::modf(nearBound + totalLength, totalLength);

    f32 low, high;
    if (al::isRailGoingToEnd(actor)) {
        low = actorCoord;
        high = wrapped;
    } else {
        low = wrapped;
        high = actorCoord;
    }

    bool isLoop = al::isLoopRail(actor);
    f32 otherCoord = al::getRailCoord(otherActor);
    if (isLoop && high < low) {
        if (low < otherCoord)
            return true;
    } else {
        if (low >= otherCoord)
            return false;
    }

    return al::getRailCoord(otherActor) < high;
}

bool TrafficRailWatcher::tryStopByOtherNpc(const al::LiveActor* actor) {
    TrafficRailActorInfo* actorInfo = nullptr;
    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i]->actor == actor) {
            actorInfo = mActors[i];
            break;
        }
    }

    for (s32 j = 0; j < mActorCount; j++) {
        if (mActors[j] != actorInfo) {
            if (isNearOnTrafficRail(actor, mActors[j])) {
                actorInfo->status = 2;
                return true;
            }
        }
    }
    return false;
}

bool TrafficRailWatcher::tryRestartByOtherNpc(const al::LiveActor* actor) {
    TrafficRailActorInfo* actorInfo = nullptr;
    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i]->actor == actor) {
            actorInfo = mActors[i];
            break;
        }
    }

    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i] == actorInfo)
            continue;
        if (isNearOnTrafficRail(actor, mActors[i]))
            return false;
    }
    actorInfo->status = 0;
    return true;
}
