#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Project/HitSensor/SensorHitGroup.h"

namespace al {
class LiveActor;
class HitSensorDirectror;
class HitSensorKeeper;

using SensorSortCmpFunc = bool (*)(al::HitSensor* a, al::HitSensor* b);

enum class HitSensorType : u32 {
    Eye = 0,
    Player = 1,
    PlayerAttack = 2,
    PlayerFoot = 3,
    PlayerDecoration = 4,
    PlayerEye = 5,
    Npc = 6,
    Ride = 7,
    Enemy = 8,
    EnemyBody = 9,
    EnemyAttack = 10,
    EnemySimple = 11,
    MapObj = 12,
    MapObjSimple = 13,
    Bindable = 14,
    CollisionParts = 15,
    PlayerFireBall = 16,
    HoldObj = 17,
    LookAt = 18,
    BindableGoal = 19,
    BindableAllPlayer = 20,
    BindableBubbleOutScreen = 21,
    BindableKoura = 22,
    BindableRouteDokan = 23,
    BindableBubblePadInput = 24
};

class HitSensor {
public:
    HitSensor(LiveActor* parentActor, const char* name, u32 hitSensorType, f32 radius,
              u16 maxSensorCount, const sead::Vector3f* followPos,
              const sead::Matrix34f* followMatrix, const sead::Vector3f& offset);

    bool trySensorSort();
    void setFollowPosPtr(const sead::Vector3f*);
    void setFollowMtxPtr(const sead::Matrix34f*);
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void update();
    void addHitSensor(HitSensor*);

    const sead::Vector3f& getOffset() const { return mOffset; }

    float getRadius() const { return mRadius; }

    void setOffset(const sead::Vector3f& offset) { mOffset.set(offset); }

    void setRadius(float radius) { mRadius = radius; }

private:
    const char* mName;
    HitSensorType mSensorType;
    sead::Vector3f mPos = {.0f, .0f, .0f};
    f32 mRadius;
    u16 mMaxSensorCount;
    u16 mSensorCount = 0;
    HitSensor** mSensors = nullptr;
    SensorSortCmpFunc* mSortFunctionPtr = nullptr;
    SensorHitGroup* mHitGroup = nullptr;
    bool mIsValidBySystem = false;
    bool mIsValid = true;
    bool _3a[4];
    u16 _3e;
    LiveActor* mParentActor;
    const sead::Vector3f* mFollowPos;
    const sead::Matrix34f* mFollowMtx;
    sead::Vector3f mOffset;
};
}  // namespace al
