#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/CollisionResultBuffer.h"

namespace al {
class HitSensor;
class KCollisionServer;
class LiveActor;
class TriangleFilterBase;

class CollisionParts {
public:
    CollisionParts(void*, const void*);

    const LiveActor* getConnectedHost() const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;
    void initParts(const sead::Matrix34f&);
    void resetAllMtx(const sead::Matrix34f&);
    f32 makeEqualScale(sead::Matrix34f*);
    void syncMtx();
    void syncMtx(const sead::Matrix34f&);

    s32 checkStrikeArrow(al::ArrowHitResultBuffer *,sead::Vector3f const&,sead::Vector3f const&,al::TriangleFilterBase const*);

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }
    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }
    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }
    const KCollisionServer& getKCollisionServer() const { return *mKCollisionServer; }
    int getSomeCounter() const { return someCounter; }
    bool isMoving() const { return mIsMoving; }
    const HitSensor* getConnectedSensor() const { return mConnectedSensor; }


    ~CollisionParts() {
        delete mKCollisionServer;
    }

public:
    void* unk[2] = {nullptr, nullptr};
    CollisionParts* selfReference = this;
    void* mPartsList = nullptr;
    sead::Matrix34f* mJointMtx = nullptr;
    sead::Matrix34f mSyncMtx;
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    sead::Matrix34f mPrevBaseInvMtx;
    sead::Vector3f mMtxScaleVec = {1.0f, 1.0f, 1.0f};
    f32 mMtxScale = 1.0f;
    f32 mInvMtxScale = 1.0f;
    s32 mPriority = -1;
    KCollisionServer* mKCollisionServer = nullptr;
    al::HitSensor* mConnectedSensor = nullptr;
    const char* mSpecialPurpose = nullptr;
    const char* mOptionalPurpose = nullptr;
    sead::Vector3f mMtxScaleVecAgain = {1.0f, 1.0f, 1.0f};
    int someCounter = 0;
    float mBoundingSphereRange = -1.0f;
    float mBaseMtxScale = 1.0f;
    bool isValidatedByUser = true;
    bool isValidatedBySystem = true;
    bool bVar3 = true;
    bool bVar4 = false;
    bool bVar5 = true;
    bool mIsMoving = false;
    char something = 0;
    bool pad2;
};
static_assert(sizeof(CollisionParts) == 0x170);

}  // namespace al
