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

    void init(const sead::Matrix34f*, const sead::Matrix34f&, HitSensor*);
    HitSensor* getConnectingSensor() const;

private:
    HitSensor* mHitSensor;
};

static_assert(sizeof(SensorConnector) == 0x68);

}  // namespace al
