#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class MtxConnector;
}  // namespace al

class TouchTargetInfo {
public:
    void setInfoBySensor(const al::HitSensor*, const sead::Vector3f&, const sead::Vector3f&);
    void setInfoByConnector(const al::MtxConnector*, const sead::Vector3f&, const sead::Vector3f&,
                            bool);
};
