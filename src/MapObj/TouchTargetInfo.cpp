#include "MapObj/TouchTargetInfo.h"

#include "Library/Collision/PartsMtxConnector.h"
#include "Library/LiveActor/ActorSensorUtil.h"

TouchTargetInfo::TouchTargetInfo() {
    reset();
}

void TouchTargetInfo::setInfoBySensor(const al::HitSensor* sensor, const sead::Vector3f& offset,
                                      const sead::Vector3f& normal) {
    reset();
    mType = Type::Sensor;
    mIsKeepTarget = true;
    mSensor = sensor;
    mSensorOffset = offset;
    mPos.setAdd(al::getSensorPos(sensor), mSensorOffset);
    mNormal.set(normal);
}

void TouchTargetInfo::reset() {
    mType = Type::PosAndNrm;
    mIsKeepTarget = false;
    mSensor = nullptr;
    mConnector = nullptr;
    mSensorOffset.set(0.0f, 0.0f, 0.0f);
    mPos.set(0.0f, 0.0f, 0.0f);
    mNormal.set(0.0f, 0.0f, 0.0f);
}

void TouchTargetInfo::setInfoByConnector(const al::MtxConnector* connector,
                                         const sead::Vector3f& pos, const sead::Vector3f& normal,
                                         bool isKeepTarget) {
    reset();
    mType = Type::Connector;
    mIsKeepTarget = isKeepTarget;
    mConnector = connector;
    mPos = pos;
    mNormal = normal;
}

void TouchTargetInfo::setInfoByPosAndNrm(const sead::Vector3f& pos, const sead::Vector3f& normal) {
    reset();
    mType = Type::PosAndNrm;
    mPos = pos;
    mNormal = normal;
}

void TouchTargetInfo::calcCurrentPosAndNrm(sead::Vector3f* pos, sead::Vector3f* normal) const {
    switch (mType) {
    case Type::PosAndNrm:
        if (pos)
            pos->set(mPos);
        if (normal)
            normal->set(mNormal);
        break;
    case Type::Sensor:
        if (pos)
            pos->setAdd(al::getSensorPos(mSensor), mSensorOffset);
        if (normal)
            normal->set(mNormal);
        break;
    case Type::Connector:
        if (pos)
            mConnector->multTrans(pos, mPos);
        if (normal)
            mConnector->multVec(normal, mNormal);
        break;
    }
}
