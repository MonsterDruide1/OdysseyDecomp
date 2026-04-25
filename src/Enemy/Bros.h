#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class BrosWeaponBase : public al::LiveActor {
public:
    BrosWeaponBase(const char* name, const al::LiveActor* bros);

    // TODO: Find what unknown does
    virtual void shoot(const sead::Vector3f& trans, const sead::Quatf& quat,
                       const sead::Vector3f& force, bool isHack, s32 unknown, bool isFast) = 0;
    virtual void killCollide(al::HitSensor* sensor, const sead::Vector3f& trans, bool isHack) = 0;
    virtual void killEnemy() = 0;

    virtual void attach(const sead::Matrix34f* poseMtx, const sead::Vector3f& trans,
                        const sead::Vector3f& rotation, const char* actionName);
    void calcAttachMtx(sead::Matrix34f* attachMtx, const sead::Matrix34f* poseMtx,
                       const sead::Vector3f& trans, const sead::Vector3f& rotation);

    virtual u32 getBreakStep() const { return 0; }

    virtual bool isBreak() const { return false; }

    const sead::Matrix34f* getPoseMtx() const { return mPoseMtx; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    const sead::Vector3f& getRotation() const { return mRotation; }

    const LiveActor* getBrosActor() const { return mBros; }

private:
    const sead::Matrix34f* mPoseMtx = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mRotation = sead::Vector3f::zero;
    const al::LiveActor* mBros;
};

static_assert(sizeof(BrosWeaponBase) == 0x130);
