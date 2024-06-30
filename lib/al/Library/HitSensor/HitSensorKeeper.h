#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;

class HitSensorKeeper {
public:
    HitSensorKeeper(s32);

    HitSensor* addSensor(LiveActor*, const char*, u32, f32, u16, const sead::Vector3f*,
                         const sead::Matrix34f*, const sead::Vector3f&);
    void update();
    s32 getSensorNum() const;
    HitSensor* getSensor(s32) const;
    void attackSensor();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    HitSensor* getSensor(const char*) const;

private:
    s32 mSensorCount;
    HitSensor** mSensors;
};

static_assert(sizeof(HitSensorKeeper) == 0x10);

}  // namespace al
