#include "Library/Clipping/ClippingGroupHolder.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {

ClippingInfoGroup::ClippingInfoGroup() : mGroupId(new PlacementId()) {}

// BUG: no bounds check
void ClippingInfoGroup::registerInfo(ClippingActorInfo* clippingActorInfo) {
    mClippingInfos[mSize] = clippingActorInfo;
    mSize++;
}

void ClippingInfoGroup::removeInfo(ClippingActorInfo* clippingActorInfo) {
    for (s32 i = 0; i < mSize; i++) {
        if (mClippingInfos[i] == clippingActorInfo) {
            mClippingInfos[i] = mClippingInfos[mSize - 1];
            mSize--;
            return;
        }
    }
}

void ClippingInfoGroup::addCount() {
    mCount++;
}

void ClippingInfoGroup::allocBuffer() {
    mClippingInfos = new ClippingActorInfo*[mCount];

    for (s32 i = 0; i < mCount; i++)
        mClippingInfos[i] = nullptr;
}

void ClippingInfoGroup::setGroupId(const ClippingActorInfo* clippingActorInfo) {
    *mGroupId = *clippingActorInfo->getPlacementId();
}

bool ClippingInfoGroup::isEqualGroupId(const PlacementId* placementId) const {
    return mGroupId->isEqual(*placementId);
}

bool ClippingInfoGroup::judgeClippingAll(const ClippingJudge* clippingJudge) const {
    for (s32 i = 0; i < mSize; i++) {
        if (!isDead(mClippingInfos[i]->getLiveActor()) &&
            (isInvalidClipping(mClippingInfos[i]->getLiveActor()) ||
             !mClippingInfos[i]->judgeClipping(clippingJudge))) {
            return false;
        }
    }
    return true;
}

void ClippingInfoGroup::startClippedAll() {
    mIsClipped = true;
    for (s32 i = 0; i < mSize; i++)
        mClippingInfos[i]->startClipped();
}

void ClippingInfoGroup::endClippedAll() {
    mIsClipped = false;
    for (s32 i = 0; i < mSize; i++)
        mClippingInfos[i]->endClipped();
}

}  // namespace al
