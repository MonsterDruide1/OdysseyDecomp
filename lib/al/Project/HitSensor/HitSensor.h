#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensorDirectror;
class HitSensorKeeper;
class SensorHitGroup;
class HitSensor;

using SensorSortCmpFunc = bool (*)(HitSensor* a, HitSensor* b);

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

    void clearSensors() { mSensorCount = 0; }

    const sead::Vector3f& getFollowPosOffset() const { return mFollowPosOffset; }

    const sead::Vector3f& getPos() const { return mPos; }

    f32 getRadius() const { return mRadius; }

    void setFollowPosOffset(const sead::Vector3f& offset) { mFollowPosOffset.set(offset); }

    void setRadius(f32 radius) { mRadius = radius; }

    LiveActor* getParentActor() const { return mParentActor; }

private:
    const char* mName;
    HitSensorType mSensorType;
    sead::Vector3f mPos = {0.0f, 0.0f, 0.0f};
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
    sead::Vector3f mFollowPosOffset;

    friend class HitSensorDirector;
    friend class HitSensorKeeper;
};
}  // namespace al
