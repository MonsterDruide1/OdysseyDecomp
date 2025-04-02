#include "Item/CoinRotateCalculator.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Stage/StageResourceList.h"

CoinRotateCalculator::CoinRotateCalculator(al::LiveActor* actor) : mActor(actor) {}

inline f32 modDegree(f32 value) {
    return al::modf(value + 360.0f, 360.0f) + 0.0f;
}

inline f32 getObjAngle(al::LiveActor* actor, bool isInWater, s32 objCountOffset) {
    al::StageSyncCounter* syncObj = al::getSceneObj<al::StageSyncCounter>(actor);
    f32 objCounter = modDegree(syncObj->getCounter() + objCountOffset);

    return (isInWater ? 2.0f : 3.0f) * objCounter;
}

void CoinRotateCalculator::reset() {
    mIsInWater = al::isInWater(mActor);
    mRotate = getObjAngle(mActor, mIsInWater, mObjCountOffset);
    mLastObjAngle = mRotate;
    mForceFrames = 0;
    mForceOffset = 0.0f;
    mFishingLineOffset = 0.0f;
    mRotateOffset = 0.0f;
}

void CoinRotateCalculator::update(const sead::Vector3f& force, bool checkWater) {
    if (checkWater)
        mIsInWater = al::isInWater(mActor);

    f32 objAngle = getObjAngle(mActor, mIsInWater, mObjCountOffset);

    if (al::isNearZero(force)) {
        if (--mForceFrames <= 0) {
            mForceOffset = modDegree(mForceOffset) - 0.8f;
            if (mForceOffset < 0.0f)
                mForceOffset = 0.0f;
        } else {
            mForceOffset += mForceFrames * 0.3f;
        }
    } else {
        mForceFrames = sead::Mathi::clamp(mForceFrames + 1, 0, 120);
        mForceOffset += mForceFrames * 0.3f;
    }

    mFishingLineOffset = sead::Mathf::clampMin(mFishingLineOffset - 0.8f, 0.0f);

    if (sead::Mathf::abs(modDegree(objAngle) - modDegree(mLastObjAngle)) > 5.0f)
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
    return al::isInWater(mActor) ? 2.0f : 3.0f;
}
