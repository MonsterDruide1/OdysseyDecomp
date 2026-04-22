#include "Util/InputSeparator.h"

#include "Util/StageInputFunction.h"

InputSeparator::InputSeparator(const al::IUseSceneObjHolder* sceneObjHolder, bool isVertical) {
    mIsVertical = isVertical;
    mSceneObjHolder = sceneObjHolder;
    mIsDominant = false;
    mDominantBorder = 8;
    mDominantTimer = 0;
}

void InputSeparator::reset() {
    mIsDominant = false;
    mDominantTimer = 0;
}

void InputSeparator::update() {
    s32 dominantTimer = mDominantTimer - 1;
    mIsDominant = false;

    if (dominantTimer >= 0)
        mDominantTimer = dominantTimer;

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
    s32 dominantTimer = mDominantTimer - 1;
    mIsDominant = false;

    if (dominantTimer >= 0)
        mDominantTimer = dominantTimer;

    if (rs::isTriggerIncrementPostProcessingFilterPreset(mSceneObjHolder) ||
        rs::isTriggerDecrementPostProcessingFilterPreset(mSceneObjHolder))
        mDominantTimer = mDominantBorder;
}

bool InputSeparator::isTriggerUiLeft() {
    if (checkDominant(false))
        return false;

    return rs::isTriggerUiLeft(mSceneObjHolder);
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
    if (checkDominant(false))
        return false;

    return rs::isTriggerUiRight(mSceneObjHolder);
}

bool InputSeparator::isTriggerUiUp() {
    if (checkDominant(true))
        return false;

    return rs::isTriggerUiUp(mSceneObjHolder);
}

bool InputSeparator::isTriggerUiDown() {
    if (checkDominant(true))
        return false;

    return rs::isTriggerUiDown(mSceneObjHolder);
}

bool InputSeparator::isHoldUiLeft() {
    if (checkDominant(false))
        return false;

    return rs::isHoldUiLeft(mSceneObjHolder);
}

bool InputSeparator::isHoldUiRight() {
    if (checkDominant(false))
        return false;

    return rs::isHoldUiRight(mSceneObjHolder);
}

bool InputSeparator::isHoldUiUp() {
    if (checkDominant(true))
        return false;

    return rs::isHoldUiUp(mSceneObjHolder);
}

bool InputSeparator::isHoldUiDown() {
    if (checkDominant(true))
        return false;

    return rs::isHoldUiDown(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiLeft() {
    if (checkDominant(false))
        return false;

    return rs::isRepeatUiLeft(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiRight() {
    if (checkDominant(false))
        return false;

    return rs::isRepeatUiRight(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiUp() {
    if (checkDominant(true))
        return false;

    return rs::isRepeatUiUp(mSceneObjHolder);
}

bool InputSeparator::isRepeatUiDown() {
    if (checkDominant(true))
        return false;

    return rs::isRepeatUiDown(mSceneObjHolder);
}

bool InputSeparator::isTriggerSnapShotMode() {
    if (checkDominant(true))
        return false;

    return rs::isTriggerSnapShotMode(mSceneObjHolder);
}

bool InputSeparator::isTriggerIncrementPostProcessingFilterPreset() {
    if (checkDominant(false))
        return false;

    return rs::isTriggerIncrementPostProcessingFilterPreset(mSceneObjHolder);
}

bool InputSeparator::isTriggerDecrementPostProcessingFilterPreset() {
    if (checkDominant(false))
        return false;

    return rs::isTriggerDecrementPostProcessingFilterPreset(mSceneObjHolder);
}
