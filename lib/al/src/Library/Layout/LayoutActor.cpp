#include "Library/Layout/LayoutActor.h"

#include "Library/Audio/AudioKeeper.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutPartsActorKeeper.h"
#include "Library/Layout/LayoutSceneInfo.h"

namespace al {
void LayoutActor::appear() {
    mIsAlive = true;
    if (mAudioKeeper)
        mAudioKeeper->appear();
    if (mLayoutPartsActorKeeper)
        mLayoutPartsActorKeeper->appear();
    updateLayoutPaneRecursive(this);
    this->calcAnim(false);
}
void LayoutActor::initLayoutPartsActorKeeper(s32 capacity) {
    mLayoutPartsActorKeeper = new LayoutPartsActorKeeper(capacity);
}
void LayoutActor::initLayoutKeeper(LayoutKeeper* layoutKeeper) {
    mLayoutKeeper = layoutKeeper;
}
NerveKeeper* LayoutActor::getNerveKeeper() const {
    return mNerveKeeper;
}
const char* LayoutActor::getName() const {
    return mName.cstr();
}
EffectKeeper* LayoutActor::getEffectKeeper() const {
    return mEffectKeeper;
}
AudioKeeper* LayoutActor::getAudioKeeper() const {
    return mAudioKeeper;
}
LayoutActionKeeper* LayoutActor::getLayoutActionKeeper() const {
    return mLayoutActionKeeper;
}
LayoutKeeper* LayoutActor::getLayoutKeeper() const {
    return mLayoutKeeper;
}
CameraDirector* LayoutActor::getCameraDirector() const {
    return mLayoutSceneInfo->getCameraDirector();
}
SceneObjHolder* LayoutActor::getSceneObjHolder() const {
    return mLayoutSceneInfo->getSceneObjHolder();
}
const MessageSystem* LayoutActor::getMessageSystem() const {
    return mLayoutSceneInfo->getMessageSystem();
}
}  // namespace al
