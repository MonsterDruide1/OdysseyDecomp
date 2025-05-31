#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class ActorPoseKeeperBase {
public:
    ActorPoseKeeperBase();

    virtual const sead::Vector3f& getRotate() const { return sead::Vector3f::zero; }

    virtual const sead::Vector3f& getScale() const { return sead::Vector3f::ones; }

    virtual const sead::Vector3f& getVelocity() const { return sead::Vector3f::zero; }

    virtual const sead::Vector3f& getFront() const { return sead::Vector3f::ez; }

    virtual const sead::Vector3f& getUp() const { return sead::Vector3f::ey; }

    virtual const sead::Quatf& getQuat() const { return sead::Quatf::unit; }

    virtual const sead::Vector3f& getGravity() const { return sDefaultVelocity; }

    virtual const sead::Matrix34f& getMtx() const { return sead::Matrix34f::ident; }

    virtual sead::Vector3f* getRotatePtr() { return nullptr; }

    virtual sead::Vector3f* getScalePtr() { return nullptr; }

    virtual sead::Vector3f* getVelocityPtr() { return nullptr; }

    virtual sead::Vector3f* getFrontPtr() { return nullptr; }

    virtual sead::Vector3f* getUpPtr() { return nullptr; }

    virtual sead::Quatf* getQuatPtr() { return nullptr; }

    virtual sead::Vector3f* getGravityPtr() { return nullptr; }

    virtual sead::Matrix34f* getMtxPtr() { return nullptr; }

    virtual void updatePoseTrans(const sead::Vector3f& trans) = 0;
    virtual void updatePoseRotate(const sead::Vector3f& rot) = 0;
    virtual void updatePoseQuat(const sead::Quatf& quat) = 0;
    virtual void updatePoseMtx(const sead::Matrix34f* mtx) = 0;
    virtual void copyPose(const ActorPoseKeeperBase* other);
    virtual void calcBaseMtx(sead::Matrix34f* mtx) const = 0;

    const sead::Vector3f& getTrans() const { return mTrans; }

    sead::Vector3f* getTransPtr() { return &mTrans; }

protected:  // protected so it's visible to all sub-classes (TFSV, TFGSV, ...)
    sead::Vector3f mTrans = {0, 0, 0};

    static sead::Vector3f sDefaultVelocity;
};

class ActorPoseKeeperTFSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTFSV();

    const sead::Vector3f& getFront() const override { return mFront; }

    sead::Vector3f* getFrontPtr() override { return &mFront; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

protected:  // protected so it's visible to all sub-classes (TFGSV, TFUSV)
    sead::Vector3f mFront = sead::Vector3f::ez;
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
};

class ActorPoseKeeperTFGSV : public ActorPoseKeeperTFSV {
public:
    ActorPoseKeeperTFGSV();

    const sead::Vector3f& getGravity() const override { return mGravity; }

    sead::Vector3f* getGravityPtr() override { return &mGravity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mGravity = {0.0, -1.0, 0.0};
};

class ActorPoseKeeperTFUSV : public ActorPoseKeeperTFSV {
public:
    ActorPoseKeeperTFUSV();

    const sead::Vector3f& getUp() const override { return mUp; }

    sead::Vector3f* getUpPtr() override { return &mUp; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

    void setFrontUp(bool isFrontUp) { mIsFrontUp = isFrontUp; }

private:
    sead::Vector3f mUp = sead::Vector3f::ey;
    bool mIsFrontUp = false;
};

class ActorPoseKeeperTQSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQSV();

    const sead::Quatf& getQuat() const override { return mQuat; }

    sead::Quatf* getQuatPtr() override { return &mQuat; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQGSV();

    const sead::Quatf& getQuat() const override { return mQuat; }

    sead::Quatf* getQuatPtr() override { return &mQuat; }

    const sead::Vector3f& getGravity() const override { return mGravity; }

    sead::Vector3f* getGravityPtr() override { return &mGravity; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity = {0.0, -1.0, 0.0};
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTQGMSV();

    const sead::Quatf& getQuat() const override { return mQuat; }

    sead::Quatf* getQuatPtr() override { return &mQuat; }

    const sead::Vector3f& getGravity() const override { return mGravity; }

    sead::Vector3f* getGravityPtr() override { return &mGravity; }

    const sead::Matrix34f& getMtx() const override { return mMtx; }

    sead::Matrix34f* getMtxPtr() override { return &mMtx; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; };

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity = {0.0, -1.0, 0.0};
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
};

class ActorPoseKeeperTRSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRSV();

    const sead::Vector3f& getRotate() const override { return mRotate; }

    sead::Vector3f* getRotatePtr() override { return &mRotate; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate = {0.0, 0.0, 0.0};
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
};

class ActorPoseKeeperTRMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRMSV();

    const sead::Vector3f& getRotate() const override { return mRotate; }

    sead::Vector3f* getRotatePtr() override { return &mRotate; }

    const sead::Matrix34f& getMtx() const override { return mMtx; }

    sead::Matrix34f* getMtxPtr() override { return &mMtx; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    __attribute__((flatten)) void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate = {0.0, 0.0, 0.0};
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
    sead::Matrix34f mMtx;  // manually set in the ctor
};

class ActorPoseKeeperTRGMSV : public ActorPoseKeeperBase {
public:
    ActorPoseKeeperTRGMSV();

    const sead::Vector3f& getRotate() const override { return mRotate; }

    sead::Vector3f* getRotatePtr() override { return &mRotate; }

    const sead::Vector3f& getGravity() const override { return mGravity; }

    sead::Vector3f* getGravityPtr() override { return &mGravity; }

    const sead::Matrix34f& getMtx() const override { return mMtx; }

    sead::Matrix34f* getMtxPtr() override { return &mMtx; }

    const sead::Vector3f& getScale() const override { return mScale; }

    sead::Vector3f* getScalePtr() override { return &mScale; }

    const sead::Vector3f& getVelocity() const override { return mVelocity; }

    sead::Vector3f* getVelocityPtr() override { return &mVelocity; }

    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;

private:
    sead::Vector3f mRotate = {0.0, 0.0, 0.0};
    sead::Vector3f mGravity = -sead::Vector3f::ey;
    sead::Vector3f mScale = {1.0, 1.0, 1.0};
    sead::Vector3f mVelocity = {0.0, 0.0, 0.0};
    sead::Matrix34f mMtx;
};

}  // namespace al
