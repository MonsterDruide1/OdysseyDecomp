#pragma once

#include <math/seadMatrix.h>

#include "Library/Collision/PartsMtxConnector.h"

namespace al {
class HitSensor;

class SensorConnector : public MtxConnector {
public:
    SensorConnector();

    bool isConnecting() const override;
    void clear() override;

    void init(const sead::Matrix34f* parentMtx, const sead::Matrix34f& mtx, HitSensor* sensor);
    HitSensor* getConnectingSensor() const;

private:
    HitSensor* mHitSensor = nullptr;
};

static_assert(sizeof(SensorConnector) == 0x68);

}  // namespace al
