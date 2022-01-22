#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class ActorPoseKeeperBase {
public:
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
    virtual void updatePoseTrans(const sead::Vector3f&);
    virtual void updatePoseRotate(const sead::Vector3f&);
    virtual void updatePoseQuat(const sead::Quatf&);
    virtual void updatePoseMtx(const sead::Matrix34f&);
    virtual void copyPose(const ActorPoseKeeperBase*);
    virtual void calcBaseMtx(sead::Matrix34f*);
private:
    sead::Vector3f mTrans{0, 0, 0};
};

class ActorPoseKeeperTFSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Vector3f& getFront() const override;
    virtual sead::Vector3f* getFrontPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mFront = sead::Vector3f::ez;
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTFGSV : public ActorPoseKeeperTFSV {
public:
    virtual const sead::Vector3f& getGravity() const override;
    virtual sead::Vector3f* getGravityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
};

class ActorPoseKeeperTFUSV : public ActorPoseKeeperTFSV {
public:
    virtual const sead::Vector3f& getUp() const override;
    virtual sead::Vector3f* getUpPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mUp = sead::Vector3f::ey;
};

class ActorPoseKeeperTQSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Quatf& getQuat() const override;
    virtual sead::Quatf* getQuatPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Quatf& getQuat() const override;
    virtual sead::Quatf* getQuatPtr() override;
    virtual const sead::Vector3f& getGravity() const override;
    virtual sead::Vector3f* getGravityPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTQGMSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Quatf& getQuat() const override;
    virtual sead::Quatf* getQuatPtr() override;
    virtual const sead::Vector3f& getGravity() const override;
    virtual sead::Vector3f* getGravityPtr() override;
    virtual const sead::Matrix34f& getMtx() const override;
    virtual sead::Matrix34f* getMtxPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
};

class ActorPoseKeeperTRSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Vector3f& getRotate() const override;
    virtual sead::Vector3f* getRotatePtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mRotate; // y and z = 0, x = uninitialized?
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
};

class ActorPoseKeeperTRMSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Vector3f& getRotate() const override;
    virtual sead::Vector3f* getRotatePtr() override;
    virtual const sead::Matrix34f& getMtx() const override;
    virtual sead::Matrix34f* getMtxPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mRotate; // y and z = 0, x = uninitialized?
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
};

class ActorPoseKeeperTRGMSV : public ActorPoseKeeperBase {
public:
    virtual const sead::Vector3f& getRotate() const override;
    virtual sead::Vector3f* getRotatePtr() override;
    virtual const sead::Vector3f& getGravity() const override;
    virtual sead::Vector3f* getGravityPtr() override;
    virtual const sead::Matrix34f& getMtx() const override;
    virtual sead::Matrix34f* getMtxPtr() override;
    virtual const sead::Vector3f& getScale() const override;
    virtual sead::Vector3f* getScalePtr() override;
    virtual const sead::Vector3f& getVelocity() const override;
    virtual sead::Vector3f* getVelocityPtr() override;
    virtual void updatePoseTrans(const sead::Vector3f&) override;
    virtual void updatePoseRotate(const sead::Vector3f&) override;
    virtual void updatePoseQuat(const sead::Quatf&) override;
    virtual void updatePoseMtx(const sead::Matrix34f&) override;
    virtual void calcBaseMtx(sead::Matrix34f*) override;
private:
    sead::Vector3f mRotate; // y and z = 0, x = uninitialized?
    sead::Vector3f mGravity{0.0, -1.0, 0.0};
    sead::Vector3f mScale{1.0, 1.0, 1.0};
    sead::Vector3f mVelocity{0.0, 0.0, 0.0};
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
};

} // namespace al
