#include "Library/Layout/LayoutActor.h"

#include "Library/Audio/AudioKeeper.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutPartsActorKeeper.h"

namespace al {
void LayoutActor::appear() {
    mIsAlive = true;
    if (mAudioKeeper)
        mAudioKeeper->appear();
    if (mLayoutPartsActorKeeper)
        mLayoutPartsActorKeeper->appear();
    updateLayoutPaneRecursive(this);
    calcAnim(false);
}

void LayoutActor::initLayoutPartsActorKeeper(s32 capacity) {
    mLayoutPartsActorKeeper = new LayoutPartsActorKeeper(capacity);
}

void LayoutActor::initLayoutKeeper(LayoutKeeper* layoutKeeper) {
    mLayoutKeeper = layoutKeeper;
}
}  // namespace al
