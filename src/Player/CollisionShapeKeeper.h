#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

class CollisionShapeInfoBase;
class CollisionShapeInfoArrow;
class CollisionShapeInfoSphere;
class CollisionShapeInfoDisk;
class CollidedShapeResult;

class CollisionShapeKeeper {
public:
    CollisionShapeKeeper(s32, s32, s32);
    void createShapeArrow(const char*, const sead::Vector3f&, const sead::Vector3f&, f32, s32);
    void createShapeSphere(const char*, f32, const sead::Vector3f&);
    void createShapeSphereSupportGround(const char*, f32, const sead::Vector3f&,
                                        const sead::Vector3f&, f32);
    void createShapeSphereIgnoreGround(const char*, f32, const sead::Vector3f&);
    void createShapeDisk(const char*, f32, const sead::Vector3f&, const sead::Vector3f&, f32);
    void createShapeDiskSupportGround(const char*, f32, const sead::Vector3f&,
                                      const sead::Vector3f&, f32, const sead::Vector3f&, f32);
    void createShapeDiskIgnoreGround(const char*, f32, const sead::Vector3f&,
                                     const sead::Vector3f&, f32);
    void updateShape();
    void clearResult();
    void calcWorldShapeInfo(const sead::Matrix34f&, f32);
    void calcRelativeShapeInfo(const sead::Matrix34f&);
    void registerCollideResult(const CollidedShapeResult&);
    void registerCollideSupportResult(const CollidedShapeResult&);
    bool isCollidedResultFull() const;
    bool isCollidedSupportResultFull() const;
    bool isShapeArrow(s32) const;
    bool isShapeSphere(s32) const;
    bool isShapeDisk(s32) const;
    const CollisionShapeInfoBase* getShapeInfoBase(s32) const;
    const CollisionShapeInfoArrow* getShapeInfoArrow(s32) const;
    const CollisionShapeInfoSphere* getShapeInfoSphere(s32) const;
    const CollisionShapeInfoDisk* getShapeInfoDisk(s32) const;
    u32 findShapeInfoIndex(const char*) const;
    const CollidedShapeResult* getCollidedShapeResult(s32) const;
    const CollidedShapeResult* getCollidedShapeSupportResult(s32) const;

public:
    sead::Vector3f mBoundingCenter = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadius = 0.0f;
    sead::PtrArray<CollisionShapeInfoBase> mCollisionShape;
    s32 mNumCollideResult = 0;
    s32 pad;
    sead::PtrArray<CollidedShapeResult> mCollideShapeResult;
    s32 mNumCollideSupportResult = 0;
    s32 pad2;
    sead::PtrArray<CollidedShapeResult> mCollideSupportResult;
    f32 mCheckStepRange = 35.0f;
    f32 unk3 = 0.0f;
    f32 unk4 = 0.0f;
    bool someFlag = false;
};
