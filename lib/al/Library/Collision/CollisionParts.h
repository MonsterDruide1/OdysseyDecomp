#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class KCollisionServer;
class LiveActor;
class HitSensor;

class CollisionParts {
public:
    const LiveActor* getConnectedHost() const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }

    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }

    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }

    KCollisionServer* getKCollisionServer() const { return mKCollisionServer; }

    s32 get_15c() const { return _15c; }

    bool isMoving() const { return mIsMoving; }

    const HitSensor* getConnectedSensor() const { return mConnectedSensor; }

private:
    void* _0[11];
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    void* _88[9];
    KCollisionServer* mKCollisionServer;
    HitSensor* mConnectedSensor = nullptr;
    void* _140[3];
    s32 _158;
    s32 _15c = 0;
    u8 _160[13];
    bool mIsMoving = false;
};

}  // namespace al
