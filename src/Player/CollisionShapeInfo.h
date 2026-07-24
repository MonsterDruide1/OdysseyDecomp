#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

enum class CollisionShapeId : u32 {
    Arrow,
    Sphere,
    Disk,
};

class CollisionShapeInfoBase : public al::HioNode {
public:
    CollisionShapeInfoBase(CollisionShapeId id, const char* name);

    virtual const sead::Vector3f& getBoundingCenter() const { return sead::Vector3f::zero; }

    virtual const sead::Vector3f& getBoundingCenterWorld() const { return sead::Vector3f::zero; }

    virtual f32 getBoundingRadius() const { return 0.0f; }

    virtual f32 getBoundingRadiusWorld() const { return 0.0f; }

    virtual f32 getCheckStepRange() const { return 100000.0f; }

    virtual f32 getCheckStepRangeWorld() const { return 100000.0f; }

    virtual void updateShapeOffset(const sead::Vector3f&) {}

    virtual void calcWorldShapeInfo(const sead::Matrix34f&, f32) {}

    virtual void calcRelativeShapeInfo(const sead::Matrix34f&) {}

    CollisionShapeId getId() const { return mId; }

    const char* getName() const { return mName; }

private:
    CollisionShapeId mId;
    const char* mName;
};

class CollisionShapeInfoArrow : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoArrow(const char*, const sead::Vector3f&, const sead::Vector3f&, f32, s32);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

private:
    u8 _18[0x1c - 0x18];
    sead::Vector3f mBoundingCenter;
    f32 mBoundingRadius;
    sead::Vector3f mBoundingCenterWorld;
    f32 mBoundingRadiusWorld;
    u8 _3c[0xb8 - 0x3c];
};

static_assert(sizeof(CollisionShapeInfoArrow) == 0xb8);

class CollisionShapeInfoSphere : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoSphere(const char*, f32, const sead::Vector3f&);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

    // TODO: rename parameter
    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        mIsSupportGround = true;
        mUp = up;
        _48 = unk1;
    }

    void setIgnoreGround() { mIsIgnoreGround = true; }

    void set48(f32 value) { _48 = value; }

private:
    f32 mBoundingRadius;
    sead::Vector3f mBoundingCenter;
    sead::Vector3f mBoundingCenterWorld;
    f32 mBoundingRadiusWorld;
    u8 _38[0x40 - 0x38];
    s32 _40;
    bool mIsSupportGround;
    f32 _48;
    f32 _4c;
    sead::Vector3f mUp;
    sead::Vector3f _5c;
    bool mIsIgnoreGround;
};

static_assert(sizeof(CollisionShapeInfoSphere) == 0x70);

class CollisionShapeInfoDisk : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoDisk(const char*, f32, const sead::Vector3f&, const sead::Vector3f&, f32);

    void updateDiskShape(f32, const sead::Vector3f&, f32);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

    // TODO: rename parameter
    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        mIsSupportGround = true;
        mUp = up;
        _84 = unk1;
    }

    void setIgnoreGround() { mIsIgnoreGround = true; }

private:
    f32 mBoundingRadius;
    f32 mBoundingRadiusWorld;
    f32 mCheckStepRange;
    f32 mCheckStepRangeWorld;
    u8 _28[0x30 - 0x28];
    sead::Vector3f mBoundingCenter;
    u8 _3c[0x50 - 0x3c];
    sead::Vector3f mBoundingCenterWorld;
    u8 _5c[0x80 - 0x5c];
    bool mIsSupportGround;
    f32 _84;
    f32 _88;
    sead::Vector3f mUp;
    sead::Vector3f _98;
    bool mIsIgnoreGround = false;
};

static_assert(sizeof(CollisionShapeInfoDisk) == 0xA8);
