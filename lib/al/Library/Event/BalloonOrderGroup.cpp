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

    s32 requesterCount = mRequesterCount;
    for (s32 i = 0; i < requesterCount; i++) {
        Requester* requesterTmp = mRequesters[i];
        if (requester->getBallonOrder() < requesterTmp->getBallonOrder()) {
            mRequesters[i] = requester;
            requesterCount = mRequesterCount;
            requester = requesterTmp;
        }
    }

    mRequesters[requesterCount] = requester;
    mRequesterCount++;
}

bool BalloonOrderGroup::isEnableAppearBalloon(const LiveActor* actor) const {
    if (!mIsActive)
        return false;

    Requester** requesters = mRequesters;

    return requesters[mActiveRequesterIdx]->getActor() == actor;
}

void BalloonOrderGroup::update(const sead::Vector3f& position) {
    mIsActive = false;

    for (s32 i = 0; i < mRequesterCount; i++) {
        if (checkInsideTerritoryPos(mRequesters[i]->getUser(), mRequesters[i]->getActor(), position,
                                    0.0f)) {
            mIsActive = true;

            break;
        }
    }

    if (!mIsActive)
        return;

    mShowStep++;

    if (mRequesters[mActiveRequesterIdx]->getBallonShowStep() > mShowStep)
        return;

    mShowStep = 0;
    mActiveRequesterIdx = mRequesterCount > mActiveRequesterIdx + 1 ? mActiveRequesterIdx + 1 : 0;
}
}  // namespace al
