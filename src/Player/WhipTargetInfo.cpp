#include "Player/WhipTargetInfo.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"

WhipTargetInfo::WhipTargetInfo() = default;

void WhipTargetInfo::initWhipTarget(al::HitSensor* sensor, const sead::Vector3f* trans) {
    mTrans = trans;
    mSensor = sensor;
}

void WhipTargetInfo::clear() {
    mTrans = nullptr;
    mSensor = nullptr;
}

bool WhipTargetInfo::isValid() const {
    return mSensor != nullptr;
}

void WhipTargetInfo::calcTargetUpDir(sead::Vector3f* upDir) {
    al::calcUpDir(upDir, al::getSensorHost(mSensor));
}

const sead::Vector3f& WhipTargetInfo::getTrans() const {
    if (mTrans)
        return *mTrans;
    return al::getSensorPos(mSensor);
}
