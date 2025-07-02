#include "Project/HitSensor/HitSensor.h"

#include "Project/HitSensor/SensorHitGroup.h"

namespace al {
void HitSensor::setFollowPosPtr(const sead::Vector3f* followPos) {
    mFollowPos = followPos;
    mFollowMtx = nullptr;
}

void HitSensor::setFollowMtxPtr(const sead::Matrix34f* followMtx) {
    mFollowPos = nullptr, mFollowMtx = followMtx;
}

void HitSensor::validate() {
    if (!mIsValid) {
        mIsValid = true;

        if (mMaxSensorCount) {
            if (mIsValidBySystem)
                mHitGroup->add(this);
        }
    }

    mSensorCount = 0;
}

void HitSensor::invalidate() {
    if (mIsValid) {
        mIsValid = false;

        if (mMaxSensorCount) {
            if (mIsValidBySystem)
                mHitGroup->remove(this);
        }
    }

    mSensorCount = 0;
}

// HitSensor::validateBySystem
// HitSensor::invalidateBySystem
// HitSensor::update (requires some more LiveActor research)

void HitSensor::addHitSensor(HitSensor* sensor) {
    // todo -- registers are different
    if (mSensorCount < mMaxSensorCount) {
        mSensors[mSensorCount] = sensor;
        ++mSensorCount;
    }
}
}  // namespace al
