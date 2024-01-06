#include "Project/HitSensor/HitSensor.h"

namespace al {
void HitSensor::setFollowPosPtr(const sead::Vector3<f32>* pFollowPos) {
    mFollowPos = pFollowPos;
    mFollowMtx = nullptr;
}

void HitSensor::setFollowMtxPtr(const sead::Matrix34<f32>* pFollowMtx) {
    mFollowPos = nullptr, mFollowMtx = pFollowMtx;
}

void HitSensor::validate() {
    if (!mIsValid) {
        mIsValid = true;

        if (mMaxSensorCount) {
            if (mIsValidBySystem) {
                mHitGroup->add(this);
            }
        }
    }

    mSensorCount = 0;
}

void HitSensor::invalidate() {
    if (mIsValid) {
        mIsValid = false;

        if (mMaxSensorCount) {
            if (mIsValidBySystem) {
                mHitGroup->remove(this);
            }
        }
    }

    mSensorCount = 0;
}

// HitSensor::validateBySystem
// HitSensor::invalidateBySystem
// HitSensor::update (requires some more LiveActor research)

void HitSensor::addHitSensor(HitSensor* pSensor) {
    // todo -- registers are different
    if (mSensorCount < mMaxSensorCount) {
        mSensors[mSensorCount] = pSensor;
        ++mSensorCount;
    }
}
}  // namespace al
