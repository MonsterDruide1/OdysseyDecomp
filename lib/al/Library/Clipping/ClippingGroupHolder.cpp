#include "Library/Clipping/ClippingGroupHolder.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {

ClippingInfoGroup::ClippingInfoGroup() : groupId(new PlacementId()) {}

// BUG: no bounds check
void ClippingInfoGroup::registerInfo(ClippingActorInfo* clippingActorInfo) {
    clippingInfos[size] = clippingActorInfo;
    size++;
}

void ClippingInfoGroup::removeInfo(ClippingActorInfo* clippingActorInfo) {
    for (s32 i = 0; i < size; i++) {
        if (clippingInfos[i] == clippingActorInfo) {
            clippingInfos[i] = clippingInfos[size - 1];
            size--;
            return;
        }
    }
}

void ClippingInfoGroup::addCount() {
    count++;
}

void ClippingInfoGroup::allocBuffer() {
    clippingInfos = new ClippingActorInfo*[count];

    for (s32 i = 0; i < count; i++)
        clippingInfos[i] = nullptr;
}

void ClippingInfoGroup::setGroupId(const ClippingActorInfo* clippingActorInfo) {
    *groupId = *clippingActorInfo->getPlacementId();
}

bool ClippingInfoGroup::isEqualGroupId(const PlacementId* placementId) const {
    return groupId->isEqual(*placementId);
}

bool ClippingInfoGroup::judgeClippingAll(const ClippingJudge* clippingJudge) const {
    for (s32 i = 0; i < size; i++) {
        if (!isDead(clippingInfos[i]->getLiveActor()) &&
            (isInvalidClipping(clippingInfos[i]->getLiveActor()) ||
             !clippingInfos[i]->judgeClipping(clippingJudge))) {
            return false;
        }
    }
    return true;
}

void ClippingInfoGroup::startClippedAll() {
    isClipped = true;
    for (s32 i = 0; i < size; i++)
        clippingInfos[i]->startClipped();
}

void ClippingInfoGroup::endClippedAll() {
    isClipped = false;
    for (s32 i = 0; i < size; i++)
        clippingInfos[i]->endClipped();
}

ClippingGroupHolder::ClippingGroupHolder() {
    mGroups = new ClippingInfoGroup*[0x40];

    for (s32 i = 0; i < 0x40; i++)
        mGroups[i] = nullptr;
}

void ClippingGroupHolder::update(const ClippingJudge* clippingJudge) {
    for (s32 i = 0; i < mGroupCount; i++) {
        ClippingInfoGroup* group = mGroups[i];
        if (!group->judgeClippingAll(clippingJudge)) {
            if (group->isClipped)
                group->endClippedAll();
        } else if (!group->isClipped)
            group->startClippedAll();
    }
}

void ClippingGroupHolder::createAndCount(ClippingActorInfo* clippingActorInfo) {
    if (!clippingActorInfo->isGroupClippingInit())
        return;

    ClippingInfoGroup* clippingInfoGroup = tryFindGroup(clippingActorInfo);
    if (!clippingInfoGroup) {
        clippingInfoGroup = new ClippingInfoGroup();

        clippingInfoGroup->setGroupId(clippingActorInfo);
        mGroups[mGroupCount] = clippingInfoGroup;
        mGroupCount++;
    }
    clippingInfoGroup->count++;
}

ClippingInfoGroup* ClippingGroupHolder::tryFindGroup(const ClippingActorInfo* clippingActorInfo) {
    for (s32 i = 0; i < mGroupCount; i++)
        if (mGroups[i]->isEqualGroupId(clippingActorInfo->getPlacementId()))
            return mGroups[i];
    return nullptr;
}

void ClippingGroupHolder::allocBuffer() {
    for (s32 i = 0; i < mGroupCount; i++)
        mGroups[i]->allocBuffer();
}

void ClippingGroupHolder::registerInfo(ClippingActorInfo* clippingActorInfo) {
    ClippingInfoGroup* group = tryFindGroup(clippingActorInfo);
    if (group)
        group->registerInfo(clippingActorInfo);
}

void ClippingGroupHolder::leave(ClippingActorInfo* clippingActorInfo) {
    ClippingInfoGroup* clippingInfoGroup = tryFindGroup(clippingActorInfo);
    if (clippingInfoGroup)
        clippingInfoGroup->removeInfo(clippingActorInfo);
}

void ClippingGroupHolder::reentry(ClippingActorInfo* clippingActorInfo) {
    registerInfo(clippingActorInfo);
}

}  // namespace al
