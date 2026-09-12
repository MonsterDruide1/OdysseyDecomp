#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class MtxConnector;
}  // namespace al

class TouchTargetInfo {
public:
    TouchTargetInfo();

    void setInfoBySensor(const al::HitSensor* sensor, const sead::Vector3f& offset,
                         const sead::Vector3f& normal);
    void reset();
    void setInfoByConnector(const al::MtxConnector* connector, const sead::Vector3f& pos,
                            const sead::Vector3f& normal, bool isKeepTarget);
    void setInfoByPosAndNrm(const sead::Vector3f& pos, const sead::Vector3f& normal);
    void calcCurrentPosAndNrm(sead::Vector3f* pos, sead::Vector3f* normal) const;

private:
    enum class Type {
        PosAndNrm,
        Sensor,
        Connector,
    };

    Type mType;
    bool mIsKeepTarget;
    const al::HitSensor* mSensor;
    const al::MtxConnector* mConnector;
    sead::Vector3f mSensorOffset;
    sead::Vector3f mPos;
    sead::Vector3f mNormal;
};

static_assert(sizeof(TouchTargetInfo) == 0x40);
