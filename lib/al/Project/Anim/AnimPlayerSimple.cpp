#include "Project/Anim/AnimPlayerSimple.h"

#include <nn/g3d/AnimObj.h>
#include <nn/g3d/BoneVisibilityAnimObj.h>

#include "Project/Anim/AnimInfo.h"

namespace al {
AnimPlayerSimple::AnimPlayerSimple() {
    mModelInfo = new ModelInfo();
}

void AnimPlayerSimple::applyTo() {
    mModelInfo->boneAnimObj->Calculate();
    mModelInfo->boneAnimObj->ApplyTo(mModelInfo->modelObj);
}

bool AnimPlayerSimple::calcNeedUpdateAnimNext() {
    if (!is11())
        return false;

    applyTo();

    if (getAnimFrameRate() <= 0 || (isAnimOneTime() && isAnimEnd())) {
        setAnimToModel(nullptr);
        set11(false);
    }
    return true;
}

void AnimPlayerSimple::startAnim(const char* name) {
    mResInfo = getAnimInfoTable()->findAnimInfo(name);
    setAnimToModel(mResInfo);
    applyTo();
    set10(true);
    set11(true);
}

void AnimPlayerSimple::update() {
    if (is11() && !is10())
        mModelInfo->boneAnimObj->getFrameCtrlPtr()->update();
}

void AnimPlayerSimple::clearAnim() {
    mModelInfo->boneAnimObj->ClearResult();
}

f32 AnimPlayerSimple::getAnimFrame() const {
    return mModelInfo->boneAnimObj->getFrameCtrlPtr()->getFrame();
}

void AnimPlayerSimple::setAnimFrame(f32 frame) {
    mModelInfo->boneAnimObj->getFrameCtrlPtr()->setFrame(frame);
    applyTo();
    set10(true);
    set11(true);
}

f32 AnimPlayerSimple::getAnimFrameMax() const {
    return mModelInfo->boneAnimObj->getFrameCtrlPtr()->getFrameMax();
}

f32 AnimPlayerSimple::getAnimFrameMax(const char* name) const {
    return getAnimInfoTable()->findAnimInfo(name)->frameMax;
}

f32 AnimPlayerSimple::getAnimFrameRate() const {
    return mModelInfo->boneAnimObj->getFrameCtrlPtr()->getFrameRate();
}

void AnimPlayerSimple::setAnimFrameRate(f32 rate) {
    mModelInfo->boneAnimObj->getFrameCtrlPtr()->setFrameRate(rate);
    applyTo();
    set11(true);
}

bool AnimPlayerSimple::isAnimExist(const char* name) const {
    return getAnimInfoTable()->tryFindAnimInfo(name) != nullptr;
}

bool AnimPlayerSimple::isAnimEnd() const {
    if (!isAnimPlaying())
        return true;

    return mModelInfo->boneAnimObj->getFrameCtrlPtr()->isEnd();
}

bool AnimPlayerSimple::isAnimOneTime() const {
    return mModelInfo->boneAnimObj->getFrameCtrlPtr()->isOneTime();
}

bool AnimPlayerSimple::isAnimOneTime(const char* name) const {
    return !getAnimInfoTable()->findAnimInfo(name)->isLooping;
}

bool AnimPlayerSimple::isAnimPlaying() const {
    return mResInfo != nullptr;
}

const char* AnimPlayerSimple::getPlayingAnimName() const {
    return isAnimPlaying() ? mResInfo->name : "";
}

}  // namespace al
