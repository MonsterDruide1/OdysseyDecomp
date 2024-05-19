#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "math/seadBoundBox.h"

enum class CollisionShapeId : u32 {
    Arrow = 0,
    Sphere = 1,
    Disk = 2,
};

class CollisionShapeInfoBase : public al::HioNode {
public:
    CollisionShapeInfoBase(CollisionShapeId, const char*);

    virtual const sead::Vector3f& getBoundingCenter() const;
    virtual const sead::Vector3f& getBoundingCenterWorld() const;
    virtual f32 getBoundingRadius() const;
    virtual f32 getBoundingRadiusWorld() const;
    virtual f32 getCheckStepRange() const;
    virtual f32 getCheckStepRangeWorld() const;
    virtual void updateShapeOffset(const sead::Vector3f&);
    virtual void calcWorldShapeInfo(const sead::Matrix34f&, f32);
    virtual void calcRelativeShapeInfo(const sead::Matrix34f&);

    CollisionShapeId getId() const { return mId; }
    const char* getName() const { return mName; }

private:
    CollisionShapeId mId;
    const char* mName;
};

class CollisionShapeInfoArrow : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoArrow(const char*, const sead::Vector3f&, const sead::Vector3f&,
                            f32, s32);
    
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
    s32 a6;
    sead::Vector3f mBoundingCenter = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadius;
    sead::Vector3f mBoundingCenterWorld = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadiusWorld;
    sead::Vector3f a3;
    sead::Vector3f a4;
    f32 a5;
    sead::Vector3f vec1 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec2 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec3 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec4 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec5 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec6 = {0.0f, 0.0f, 0.0f};
    sead::BoundBox3f mAabb;
};
static_assert(sizeof(CollisionShapeInfoArrow) == 0xb8);

class CollisionShapeInfoSphere : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoSphere(char const*, float, sead::Vector3<float> const&);
    
    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        isSupportGround = true;
        mUp = up;
        this->unk1 = unk1;
    }
    void setIgnoreGround() { isIgnoreGround = true; }

private:
    float mBoundingRadius;
    sead::Vector3f mBoundingCenter;
    sead::Vector3f mBoundingCenterWorld;
    float mBoundingRadiusWorld;
    sead::Vector3f mRelativeShapeInfo = {0.0f, 0.0f, 0.0f};
    bool isSupportGround = false;
    float unk1 = 0.0f;
    float unk2;
    sead::Vector3f mUp = sead::Vector3f::ey;
    sead::Vector3f mWorldShapeInfo;
    bool isIgnoreGround = false;
    bool unkFlag = false;
};
static_assert(sizeof(CollisionShapeInfoSphere) == 0x70);

class CollisionShapeInfoDisk : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoDisk(char const*, float, sead::Vector3<float> const&,
                           sead::Vector3<float> const&, float);

    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        isSupportGround = true;
        mUp = up;
        this->unk1 = unk1;
    }
    void setIgnoreGround() { isIgnoreGround = true; }

private:
    void* size[13];
    bool isSupportGround;
    bool pad[3];
    float unk1;
    int gap3;
    sead::Vector3f mUp;
    void* size2[1];
    int gap;
    bool isIgnoreGround;
    bool flags[3];
};
static_assert(sizeof(CollisionShapeInfoDisk) == 0xA8);
