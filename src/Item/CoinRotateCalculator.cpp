#include "Item/CoinRotateCalculator.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Stage/StageResourceList.h"

#include "Scene/SceneObjFactory.h"

CoinRotateCalculator::CoinRotateCalculator(al::LiveActor* actor) : mActor(actor) {}

inline f32 convergeDegree(f32 value) {
    return al::modf(value + 360.0f, 360.0f) + 0.0f;
}

f32 CoinRotateCalculator::getObjAngle(bool isInWater, s32 objCountOffset) const {
    al::StageSyncCounter* syncObj =
        al::getSceneObj<al::StageSyncCounter>(mActor, SceneObjID_alStageSyncCounter);
    f32 objCounter = convergeDegree(syncObj->getCounter() + objCountOffset);

    return (isInWater ? 2.0f : 3.0f) * objCounter;
}

void CoinRotateCalculator::reset() {
    mIsInWater = al::isInWater(mActor);
    mRotate = getObjAngle(mIsInWater, mObjCountOffset);
    mLastObjAngle = mRotate;
    mCount = 0;
    mForceOffset = 0.0f;
    mFishingLineOffset = 0.0f;
    mRotateOffset = 0.0f;
}

// NON_MATCHING: mismatch in storing mLastObjAngle = objAngle; (two strs instead of stp)
void CoinRotateCalculator::update(const sead::Vector3f& force, bool checkWater) {
    if (checkWater)
        mIsInWater = al::isInWater(mActor);

    f32 objAngle = getObjAngle(mIsInWater, mObjCountOffset);

    if (!al::isNearZero(force, 0.001f)) {
        mCount = sead::Mathi::clamp(mCount + 1, 0, 120);
        mForceOffset += mCount * 0.3f;
    } else if (--mCount <= 0) {
        mForceOffset = convergeDegree(mForceOffset) - 0.8f;
        if (mForceOffset < 0.0f)
            mForceOffset = 0.0f;
    } else {
        mForceOffset += mCount * 0.3f;
    }

    mFishingLineOffset = sead::Mathf::clampMin(mFishingLineOffset - 0.8f, 0.0f);

    if (sead::Mathf::abs(convergeDegree(objAngle) - convergeDegree(mLastObjAngle)) > 5.0f)
        objAngle = (mIsInWater ? 3.5f : 4.0f) + mLastObjAngle;

    mRotate = objAngle + mForceOffset + mFishingLineOffset + mRotateOffset;
    mLastObjAngle = objAngle;
}

void CoinRotateCalculator::addFishingLineTouch() {
    mFishingLineOffset += 5;
}

f32 CoinRotateCalculator::getRotate() const {
    return mRotate;
}

f32 CoinRotateCalculator::getRotateSpeed() const {
    if (!al::isInWater(mActor))
        return 3.0f;
    return 2.0f;
}
