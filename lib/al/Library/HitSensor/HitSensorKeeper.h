#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;

class HitSensorKeeper {
public:
    HitSensorKeeper(s32 maxSensors);

    HitSensor* addSensor(LiveActor* parentActor, const char* name, u32 hitSensorType, f32 radius,
                         u16 maxSensorCount, const sead::Vector3f* followPos,
                         const sead::Matrix34f* followMatrix, const sead::Vector3f& offset);
    void update();
    s32 getSensorNum() const;
    HitSensor* getSensor(s32 index) const;
    void attackSensor();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    HitSensor* getSensor(const char* name) const;

private:
    s32 mMaxSensors;
    s32 mSensorCount = 0;
    HitSensor** mSensors;
};

static_assert(sizeof(HitSensorKeeper) == 0x10);

}  // namespace al
