#include "Util/InputSeparator.h"

#include "Util/StageInputFunction.h"

InputSeparator::InputSeparator(const al::IUseSceneObjHolder* sceneObjHolder, bool isVertical)
    : mSceneObjHolder(sceneObjHolder), mIsVertical(isVertical) {}

void InputSeparator::reset() {
    mIsDominant = false;
    mDominantTimer = 0;
}

void InputSeparator::update() {
    mIsDominant = false;

    if (mDominantTimer > 0)
        mDominantTimer--;

    if (mIsVertical) {
        if (rs::isHoldUiUp(mSceneObjHolder) || rs::isHoldUiDown(mSceneObjHolder)) {
            mIsDominant = true;
            mDominantTimer = mDominantBorder;
        }

        if (rs::isTriggerUiUp(mSceneObjHolder) || rs::isTriggerUiDown(mSceneObjHolder))
            mDominantTimer = mDominantBorder;

        return;
    }

    if (rs::isHoldUiRight(mSceneObjHolder) || rs::isHoldUiLeft(mSceneObjHolder)) {
        mIsDominant = true;
        mDominantTimer = mDominantBorder;
    }

    if (rs::isTriggerUiRight(mSceneObjHolder) || rs::isTriggerUiLeft(mSceneObjHolder))
        mDominantTimer = mDominantBorder;
}

void InputSeparator::updateForSnapShotMode() {
    mIsDominant = false;

    if (mDominantTimer > 0)
        mDominantTimer--;

    if (rs::isTriggerIncrementPostProcessingFilterPreset(mSceneObjHolder) ||
        rs::isTriggerDecrementPostProcessingFilterPreset(mSceneObjHolder))
        mDominantTimer = mDominantBorder;
}

bool InputSeparator::isTriggerUiLeft() {
    return !checkDominant(false) && rs::isTriggerUiLeft(mSceneObjHolder);
}

bool InputSeparator::checkDominant(bool isVertical) {
    if (mIsVertical != isVertical) {
        if (mIsDominant)
            return true;

        if (mDominantTimer > 0)
            return true;
    }

    return false;
}

bool InputSeparator::isTriggerUiRight() {
    return !checkDominant(false) && rs::isTriggerUiRight(mSceneObjHolder);
}

bool InputSeparator::isTriggerUiUp() {
    return !checkDominant(true) && rs::isTriggerUiUp(mSceneObjHolder);
}

bool InputSeparator::isTriggerUiDown() {
    return !checkDominant(true) && rs::isTriggerUiDown(mSceneObjHolder);
}

bool InputSeparator::isHoldUiLeft() {
    return !checkDominant(false) && rs::isHoldUiLeft(mSceneObjHolder);
}

bool InputSeparator::isHoldUiRight() {
    return !checkDominant(false) && rs::isHoldUiRight(mSceneObjHolder);
}

bool InputSeparator::isHoldUiUp() {
    return !checkDominant(true) && rs::isHoldUiUp(mSceneObjHolder);
}

bool InputSeparator::isHoldUiDown() {
    return !checkDominant(true) && rs::isHoldUiDown(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiLeft() {
    return !checkDominant(false) && rs::isRepeatUiLeft(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiRight() {
    return !checkDominant(false) && rs::isRepeatUiRight(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiUp() {
    return !checkDominant(true) && rs::isRepeatUiUp(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiDown() {
    return !checkDominant(true) && rs::isRepeatUiDown(mSceneObjHolder);
}

bool InputSeparator::isTriggerSnapShotMode() {
    return !checkDominant(true) && rs::isTriggerSnapShotMode(mSceneObjHolder);
}

bool InputSeparator::isTriggerIncrementPostProcessingFilterPreset() {
    return !checkDominant(false) &&
           rs::isTriggerIncrementPostProcessingFilterPreset(mSceneObjHolder);
}

bool InputSeparator::isTriggerDecrementPostProcessingFilterPreset() {
    return !checkDominant(false) &&
           rs::isTriggerDecrementPostProcessingFilterPreset(mSceneObjHolder);
}
