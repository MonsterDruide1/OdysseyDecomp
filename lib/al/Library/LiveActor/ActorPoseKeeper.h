#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

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

protected: // protected so it's visible to all sub-classes (TFSV, TFGSV, ...)
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

protected: // protected so it's visible to all sub-classes (TFGSV, TFUSV)
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
    __attribute__((flatten))
    void updatePoseTrans(const sead::Vector3f& trans) override;
    void updatePoseRotate(const sead::Vector3f& rot) override;
    void updatePoseQuat(const sead::Quatf& quat) override;
    void updatePoseMtx(const sead::Matrix34f* mtx) override;
    void calcBaseMtx(sead::Matrix34f* mtx) const override;
private:
    sead::Vector3f mRotate{0.0, 0.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx; // manually set in the ctor
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

} // namespace al
