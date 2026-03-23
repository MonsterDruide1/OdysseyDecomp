#include "Library/Event/BalloonOrderGroup.h"

#include "Library/Event/EventFlowUtil.h"
#include "Library/Placement/PlacementId.h"

namespace al {
BalloonOrderGroup::BalloonOrderGroup(const PlacementInfo& info) {
    mPlacementId = createPlacementId(info);
    mRequesters = new Requester*[8];
}

bool BalloonOrderGroup::isEqual(const PlacementInfo& info) const {
    PlacementId placementId;
    if (!tryGetPlacementId(&placementId, info))
        return false;

    return PlacementId::isEqual(placementId, *mPlacementId);
}

void BalloonOrderGroup::registerRequester(const LiveActor* actor, const IUseEventFlowData* user,
                                          const ActorInitInfo& info) {
    Requester* requester = new Requester(actor, user, info);

    for (s32 i = 0; i < mRequesterCount; i++) {
        Requester* requesterTmp = mRequesters[i];
        if (requester->ballonOrder < requesterTmp->ballonOrder) {
            mRequesters[i] = requester;
            requester = requesterTmp;
        }
    }

    mRequesters[mRequesterCount] = requester;
    mRequesterCount++;
}

bool BalloonOrderGroup::isEnableAppearBalloon(const LiveActor* actor) const {
    if (!mIsActive)
        return false;

    Requester** requesters = mRequesters;

    return requesters[mActiveRequesterIdx]->actor == actor;
}

void BalloonOrderGroup::update(const sead::Vector3f& position) {
    mIsActive = false;

    for (s32 i = 0; i < mRequesterCount; i++) {
        if (checkInsideTerritoryPos(mRequesters[i]->user, mRequesters[i]->actor, position, 0.0f)) {
            mIsActive = true;

            break;
        }
    }

    if (!mIsActive)
        return;

    mShowStep++;

    if (mRequesters[mActiveRequesterIdx]->ballonShowStep <= mShowStep) {
        mShowStep = 0;
        mActiveRequesterIdx =
            mRequesterCount > mActiveRequesterIdx + 1 ? mActiveRequesterIdx + 1 : 0;
    }
}
}  // namespace al
