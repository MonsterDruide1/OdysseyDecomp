#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Project/HitSensor/SensorHitGroup.h"

namespace al {
class LiveActor;

class HitSensor {
public:
    HitSensor(LiveActor*, const char*, u32, f32, u16, const sead::Vector3<f32>*,
              const sead::Matrix34<f32>*, const sead::Vector3<f32>&);

    bool trySensorSort();
    void setFollowPosPtr(const sead::Vector3<f32>*);
    void setFollowMtxPtr(const sead::Matrix34<f32>*);
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void update();
    void addHitSensor(HitSensor*);

    const char* mName;  // _0
    s32 _8;
    f32 _c;
    f32 _10;
    f32 _14;
    f32 _18;
    u16 mMaxSensorCount;   // _1C
    u16 mSensorCount;      // _1E
    HitSensor** mSensors;  // _20
    u64 _28;
    SensorHitGroup* mHitGroup;  // _30
    bool mIsValidBySystem;      // _38
    bool mIsValid;              // _39
    bool _3A[4];                // unknown type
    u16 _3E;
    LiveActor* mParentActor;                // _40
    const sead::Vector3<f32>* mFollowPos;   // _48
    const sead::Matrix34<f32>* mFollowMtx;  // _50
};
}  // namespace al
