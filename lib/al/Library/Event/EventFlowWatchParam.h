#pragma once

#include <math/seadVector.h>

namespace al {
class ByamlIter;
class HitSensor;
class LiveActor;

class EventFlowWatchParam {
public:
    EventFlowWatchParam();

    void load(const ByamlIter&);
    bool isWatchSensor(const HitSensor*) const;
    void calcWatchTrans(sead::Vector3f*, const LiveActor*) const;

    const char* getSensorName() const { return mSensorName; }

    const sead::Vector3f& getLocalOffset() const { return mLocalOffset; }

private:
    const char* mSensorName = nullptr;
    sead::Vector3f mLocalOffset = sead::Vector3f::zero;
};

static_assert(sizeof(EventFlowWatchParam) == 0x18);
}  // namespace al
