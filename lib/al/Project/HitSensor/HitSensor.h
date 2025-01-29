#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Project/HitSensor/SensorHitGroup.h"

namespace al {
class LiveActor;

class HitSensor {
public:
    HitSensor(LiveActor*, const char*, u32, f32, u16, const sead::Vector3f*, const sead::Matrix34f*,
              const sead::Vector3f&);

    bool trySensorSort();
    void setFollowPosPtr(const sead::Vector3f*);
    void setFollowMtxPtr(const sead::Matrix34f*);
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void update();
    void addHitSensor(HitSensor*);

    const char* mName;
    s32 _8;
    f32 _c;
    f32 _10;
    f32 _14;
    f32 _18;
    u16 mMaxSensorCount;
    u16 mSensorCount;
    HitSensor** mSensors;
    u64 _28;
    SensorHitGroup* mHitGroup;
    bool mIsValidBySystem;
    bool mIsValid;
    bool _3a[4];
    u16 _3e;
    LiveActor* mParentActor;
    const sead::Vector3f* mFollowPos;
    const sead::Matrix34f* mFollowMtx;
};
}  // namespace al
