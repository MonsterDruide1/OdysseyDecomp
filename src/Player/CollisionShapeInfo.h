#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

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
    CollisionShapeInfoArrow(char const*, sead::Vector3<float> const&, sead::Vector3<float> const&,
                            float, int);

private:
    void* size[0x14];
};
static_assert(sizeof(CollisionShapeInfoArrow) == 0xb8);

class CollisionShapeInfoSphere : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoSphere(char const*, float, sead::Vector3<float> const&);

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
    sead::Vector3f mRelativeShapeInfo;
    bool isSupportGround;
    bool pad[3];
    float unk1;
    float unk2;
    sead::Vector3f mUp;
    sead::Vector3f mWorldShapeInfo;
    bool isIgnoreGround;
    bool unkFlag;
    bool pad2[6];
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
