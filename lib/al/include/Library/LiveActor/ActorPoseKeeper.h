#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include "math/seadVectorFwd.h"

namespace al {

class ActorPoseKeeperBase {
public:
    ActorPoseKeeperBase();

    virtual const sead::Vector3f& getRotate() const;
    virtual const sead::Vector3f& getScale() const;
    virtual const sead::Vector3f& getVelocity() const;
    virtual const sead::Vector3f& getFront() const;
    virtual const sead::Vector3f& getUp() const;
    virtual const sead::Quatf& getQuat() const;
    virtual const sead::Vector3f& getGravity() const;
    virtual const sead::Matrix34f& getMtx() const;
    virtual sead::Vector3f* getRotatePtr();
    virtual sead::Vector3f* getScalePtr();
    virtual sead::Vector3f* getVelocityPtr();
    virtual sead::Vector3f* getFrontPtr();
    virtual sead::Vector3f* getUpPtr();
    virtual sead::Quatf* getQuatPtr();
    virtual sead::Vector3f* getGravityPtr();
    virtual sead::Matrix34f* getMtxPtr();
    virtual void updatePoseTrans(const sead::Vector3f& trans) = 0;
    virtual void updatePoseRotate(const sead::Vector3f& rot) = 0;
    virtual void updatePoseQuat(const sead::Quatf& quat) = 0;
    virtual void updatePoseMtx(const sead::Matrix34f* mtx) = 0;
    virtual void copyPose(const ActorPoseKeeperBase* other);
    virtual void calcBaseMtx(sead::Matrix34f* mtx) const = 0;

protected:  // protected so it's visible to all sub-classes (TFSV, TFGSV, ...)
    sead::Vector3f mTrans{0, 0, 0};

    static sead::Vector3f sDefaultVelocity;
};

class ActorPoseKeeperTFSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTFSV();

    const sead::Vector3f& getFront() const override;
    sead::Vector3f* getFrontPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

protected:  // protected so it's visible to all sub-classes (TFGSV, TFUSV)
    sead::Vector3f mFront = sead::Vector3f::ez;
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTFGSV : public ActorPoseKeeperTFSV {
public:
    ActorPoseKeeperTFGSV();

    const sead::Vector3f& getGravity() const override;
    sead::Vector3f* getGravityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
};

class ActorPoseKeeperTFUSV : public ActorPoseKeeperTFSV {
public:
    ActorPoseKeeperTFUSV();

    const sead::Vector3f& getUp() const override;
    sead::Vector3f* getUpPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mUp = sead::Vector3f::ey;
    bool mIsFrontUp = false;
};

class ActorPoseKeeperTQSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQSV();

    const sead::Quatf& getQuat() const override;
    sead::Quatf* getQuatPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQGSV();

    const sead::Quatf& getQuat() const override;
    sead::Quatf* getQuatPtr() override;
    const sead::Vector3f& getGravity() const override;
    sead::Vector3f* getGravityPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQGMSV();

    const sead::Quatf& getQuat() const override;
    sead::Quatf* getQuatPtr() override;
    const sead::Vector3f& getGravity() const override;
    sead::Vector3f* getGravityPtr() override;
    const sead::Matrix34f& getMtx() const override;
    sead::Matrix34f* getMtxPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
};

class ActorPoseKeeperTRSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRSV();

    const sead::Vector3f& getRotate() const override;
    sead::Vector3f* getRotatePtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate{0.0, 0.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTRMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRMSV();

    const sead::Vector3f& getRotate() const override;
    sead::Vector3f* getRotatePtr() override;
    const sead::Matrix34f& getMtx() const override;
    sead::Matrix34f* getMtxPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    __attribute__((flatten)) void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate{0.0, 0.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx;  // manually set in the ctor
};

class ActorPoseKeeperTRGMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRGMSV();

    const sead::Vector3f& getRotate() const override;
    sead::Vector3f* getRotatePtr() override;
    const sead::Vector3f& getGravity() const override;
    sead::Vector3f* getGravityPtr() override;
    const sead::Matrix34f& getMtx() const override;
    sead::Matrix34f* getMtxPtr() override;
    const sead::Vector3f& getScale() const override;
    sead::Vector3f* getScalePtr() override;
    const sead::Vector3f& getVelocity() const override;
    sead::Vector3f* getVelocityPtr() override;
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate{0.0, 0.0, 0.0};
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx;
};

class LiveActor;

void initActorPoseT(LiveActor* actor, const sead::Vector3f&);
void setTrans(LiveActor* actor, const sead::Vector3f&);
void initActorPoseTR(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&);
void setRotate(LiveActor* actor, const sead::Vector3f&);
void makeMtxSRT(sead::Matrix34f*, const LiveActor* actor);
void makeMtxRT(sead::Matrix34f*, const LiveActor* actor);
void makeMtxR(sead::Matrix34f*, const LiveActor* actor);
void calcAnimFrontGravityPos(LiveActor* actor, const sead::Vector3f&);
const sead::Vector3f& getGravity(const al::LiveActor* actor);
const sead::Vector3f& getTrans(const al::LiveActor* actor);
const sead::Vector3f& getScale(const al::LiveActor* actor);
void copyPose(LiveActor* actor, const LiveActor* target);
void updatePoseTrans(LiveActor* actor, const sead::Vector3f&);
void updatePoseRotate(LiveActor* actor, const sead::Vector3f&);
void updatePoseQuat(LiveActor* actor, const sead::Quatf&);
void updatePoseMtx(LiveActor* actor, const sead::Matrix34f*);
void calcSideDir(sead::Vector3f*, const LiveActor* actor);
void calcLeftDir(sead::Vector3f*, const LiveActor* actor);
void calcRightDir(sead::Vector3f*, const LiveActor* actor);
void calcUpDir(sead::Vector3f*, const LiveActor* actor);
void calcDownDir(sead::Vector3f*, const LiveActor* actor);
void calcFrontDir(sead::Vector3f*, const LiveActor* actor);
void calcBackDir(sead::Vector3f*, const LiveActor* actor);
void calcPoseDir(sead::Vector3f*, sead::Vector3f*, sead::Vector3f*, const LiveActor* actor);
void calcQuat(sead::Quatf*, const LiveActor* actor);
void calcDistanceSignLocalXDir(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceSignLocalYDir(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceSignLocalZDir(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceLocalXDir(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceLocalYDir(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceLocalZDir(const LiveActor* actor, const sead::Vector3f&);
sead::Vector3f* getTransPtr(al::LiveActor* actor);
void setTrans(LiveActor* actor, f32, f32, f32);
void setTransX(LiveActor* actor, f32);
void setTransY(LiveActor* actor, f32);
void setTransZ(LiveActor* actor, f32);
const sead::Vector3f& getRotate(const LiveActor* actor);
sead::Vector3f* getRotatePtr(LiveActor* actor);
void setRotate(LiveActor* actor, f32, f32, f32);
void setRotateX(LiveActor* actor, f32);
void setRotateY(LiveActor* actor, f32);
void setRotateZ(LiveActor* actor, f32);
void getScalePtr(LiveActor* actor);
bool tryGetScalePtr(LiveActor* actor);
f32 getScaleX(const al::LiveActor* actor);
f32 getScaleY(const al::LiveActor* actor);
f32 getScaleZ(const al::LiveActor* actor);
void setScale(LiveActor* actor, const sead::Vector3f&);
void setScale(LiveActor* actor, f32, f32, f32);
void setScaleAll(LiveActor* actor, f32);
void setScaleX(LiveActor* actor, f32);
void setScaleY(LiveActor* actor, f32);
void setScaleZ(LiveActor* actor, f32);
bool isFrontTarget(const LiveActor* actor, const sead::Vector3f&);
bool isFrontDir(const LiveActor* actor, const sead::Vector3f&);
bool isBackTarget(const LiveActor* actor, const sead::Vector3f&);
bool isBackDir(const LiveActor* actor, const sead::Vector3f&);
bool isLeftTarget(const LiveActor* actor, const sead::Vector3f&);
bool isLeftDir(const LiveActor* actor, const sead::Vector3f&);
bool isRightTarget(const LiveActor* actor, const sead::Vector3f&);
bool isRightDir(const LiveActor* actor, const sead::Vector3f&);
bool isUpTarget(const LiveActor* actor, const sead::Vector3f&);
bool isUpDir(const LiveActor* actor, const sead::Vector3f&);
bool isDownTarget(const LiveActor* actor, const sead::Vector3f&);
bool isDownDir(const LiveActor* actor, const sead::Vector3f&);
const sead::Quatf& getQuat(const al::LiveActor* actor);
sead::Quatf* getQuatPtr(al::LiveActor* actor);
bool tryGetQuatPtr(LiveActor* actor);
void setQuat(LiveActor* actor, const sead::Quatf&);
void getGravityPtr(const LiveActor* actor);
void setGravity(const LiveActor* actor, const sead::Vector3f&);
const sead::Vector3f& getFront(const al::LiveActor* actor);
sead::Vector3f* getFrontPtr(al::LiveActor* actor);
void setFront(LiveActor* actor, const sead::Vector3f&);
const sead::Vector3f& getUp(const al::LiveActor* actor);
sead::Vector3f* getUpPtr(al::LiveActor* actor);
void setUp(LiveActor* actor, const sead::Vector3f&);
void multVecPoseNoTrans(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void multVecPose(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void multVecPoseAndScale(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void multVecInvPose(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void multVecInvQuat(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void multMtxInvPose(sead::Matrix34f*, const LiveActor* actor, const sead::Matrix34f&);
void calcTransLocalOffset(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);

}  // namespace al

class alActorPoseFunction {
    void calcBaseMtx(sead::Matrix34f* mtx, const al::LiveActor* actor);
    void updatePoseTRMSV(al::LiveActor* actor);
};
