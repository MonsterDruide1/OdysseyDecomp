#include "Library/Collision/PartsSensorConnector.h"

#include "Library/LiveActor/ActorSensorUtil.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {

SensorConnector::SensorConnector() = default;

bool SensorConnector::isConnecting() const {
    return mHitSensor && isSensorValid(mHitSensor) && MtxConnector::isConnecting();
}

void SensorConnector::clear() {
    MtxConnector::clear();
    mHitSensor = nullptr;
}

void SensorConnector::init(const sead::Matrix34f* parentMtx, const sead::Matrix34f& mtx,
                           HitSensor* sensor) {
    MtxConnector::init(parentMtx, mtx);
    mHitSensor = sensor;
}

HitSensor* SensorConnector::getConnectingSensor() const {
    return mHitSensor;
}

}  // namespace al
