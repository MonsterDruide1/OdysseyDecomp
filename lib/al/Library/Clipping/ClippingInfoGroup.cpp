#include "Library/Clipping/ClippingInfoGroup.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {

ClippingInfoGroup::ClippingInfoGroup() : mCount(0), mSize(0), mClippingInfos(nullptr) {
    mGroupId = new PlacementId();
    mIsClipped = false;
}

void ClippingInfoGroup::registerInfo(ClippingActorInfo* clippingActorInfo) {
    mClippingInfos[mSize] = clippingActorInfo;
    mSize++;
}

void ClippingInfoGroup::removeInfo(ClippingActorInfo* clippingActorInfo) {
    if (mSize < 0)
        return;

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

    if (0 < mCount)
        for (s32 i = 0; i < mCount; i++)
            mClippingInfos[i] = nullptr;
}

void ClippingInfoGroup::setGroupId(const ClippingActorInfo* clippingActorInfo) {}

bool ClippingInfoGroup::isEqualGroupId(const PlacementId* placementId) const {
    return mGroupId->isEqual(*placementId);
}

bool ClippingInfoGroup::judgeClippingAll(const ClippingJudge* clippingJudge) const {
    for (s32 i = 0; i < mSize; i++) {
        if (!isDead(mClippingInfos[i]->liveActor) &&
            (isInvalidClipping(mClippingInfos[i]->liveActor) ||
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
