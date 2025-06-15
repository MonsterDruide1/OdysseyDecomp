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
    CollisionShapeKeeper(s32 maxShapes, s32 maxCollideResults, s32 maxCollideSupportResults);
    // TODO: rename parameters
    void createShapeArrow(const char* name, const sead::Vector3f& start, const sead::Vector3f& end,
                          f32 unk1, s32 unk2);
    void createShapeSphere(const char* name, f32 radius, const sead::Vector3f& position);
    // TODO: rename parameter
    void createShapeSphereSupportGround(const char* name, f32 radius, const sead::Vector3f& pos,
                                        const sead::Vector3f& up, f32 unk1);
    void createShapeSphereIgnoreGround(const char* name, f32 radius, const sead::Vector3f& pos);
    // TODO: rename parameters
    void createShapeDisk(const char* name, f32 a1, const sead::Vector3f& a2,
                         const sead::Vector3f& a3, f32 a4);
    // TODO: rename parameters
    void createShapeDiskSupportGround(const char* name, f32 a1, const sead::Vector3f& a2,
                                      const sead::Vector3f& a3, f32 a4, const sead::Vector3f& a5,
                                      f32 a6);
    // TODO: rename parameters
    void createShapeDiskIgnoreGround(const char* name, f32 a1, const sead::Vector3f& a2,
                                     const sead::Vector3f& a3, f32 a4);
    void updateShape();
    void clearResult();
    // TODO: rename parameters
    void calcWorldShapeInfo(const sead::Matrix34f& a2, f32 a3);
    // TODO: rename parameter
    void calcRelativeShapeInfo(const sead::Matrix34f& a2);
    void registerCollideResult(const CollidedShapeResult& result);
    void registerCollideSupportResult(const CollidedShapeResult& result);
    bool isCollidedResultFull() const;
    bool isCollidedSupportResultFull() const;
    bool isShapeArrow(s32 index) const;
    bool isShapeSphere(s32 index) const;
    bool isShapeDisk(s32 index) const;
    const CollisionShapeInfoBase* getShapeInfoBase(s32 index) const;
    const CollisionShapeInfoArrow* getShapeInfoArrow(s32 index) const;
    const CollisionShapeInfoSphere* getShapeInfoSphere(s32 index) const;
    const CollisionShapeInfoDisk* getShapeInfoDisk(s32 index) const;
    u32 findShapeInfoIndex(const char* name) const;
    const CollidedShapeResult* getCollidedShapeResult(s32 index) const;
    const CollidedShapeResult* getCollidedShapeSupportResult(s32 index) const;

private:
    sead::Vector3f mBoundingCenter = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadius = 0.0f;
    sead::PtrArray<CollisionShapeInfoBase> mCollisionShape;
    s32 mNumCollideResult = 0;
    sead::PtrArray<CollidedShapeResult> mCollideShapeResult;
    s32 mNumCollideSupportResult = 0;
    sead::PtrArray<CollidedShapeResult> mCollideSupportResult;
    f32 mCheckStepRange = 35.0f;
    f32 _54 = 0.0f;
    f32 _58 = 0.0f;
    bool mHasShapeArrow = false;
};

static_assert(sizeof(CollisionShapeKeeper) == 0x60);
