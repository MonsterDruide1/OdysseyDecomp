#include "Player/CollisionShapeKeeper.h"
#include "Player/CollidedShapeResult.h"

CollisionShapeKeeper::CollisionShapeKeeper(s32 maxShapes, s32 maxCollideResults, s32 maxCollideSupportResults) {
    mCollisionShape.allocBuffer(maxShapes, nullptr);

    if(maxCollideResults >= 1) {
        mCollideShapeResult.allocBuffer(maxCollideResults, nullptr);
        for(s32 i = 0; i < maxCollideResults; i++) {
            mCollideShapeResult.pushBack(new CollidedShapeResult(nullptr));
        }
    }

    if(maxCollideSupportResults >= 1) {
        mCollideSupportResult.allocBuffer(maxCollideSupportResults, nullptr);
        for(s32 i = 0; i < maxCollideSupportResults; i++) {
            mCollideSupportResult.pushBack(new CollidedShapeResult(nullptr));
        }
    }
}
void CollisionShapeKeeper::createShapeArrow(const char*, const sead::Vector3f&,
                                            const sead::Vector3f&, f32, s32) {}
void CollisionShapeKeeper::createShapeSphere(const char*, f32, const sead::Vector3f&) {}
void CollisionShapeKeeper::createShapeSphereSupportGround(const char*, f32, const sead::Vector3f&,
                                                          const sead::Vector3f&, f32) {}
void CollisionShapeKeeper::createShapeSphereIgnoreGround(const char*, f32, const sead::Vector3f&) {}
void CollisionShapeKeeper::createShapeDisk(const char*, f32, const sead::Vector3f&,
                                           const sead::Vector3f&, f32) {}
void CollisionShapeKeeper::createShapeDiskSupportGround(const char*, f32, const sead::Vector3f&,
                                                        const sead::Vector3f&, f32,
                                                        const sead::Vector3f&, f32) {}
void CollisionShapeKeeper::createShapeDiskIgnoreGround(const char*, f32, const sead::Vector3f&,
                                                       const sead::Vector3f&, f32) {}
void CollisionShapeKeeper::updateShape() {}
void CollisionShapeKeeper::clearResult() {}
void CollisionShapeKeeper::calcWorldShapeInfo(const sead::Matrix34f&, f32) {}
void CollisionShapeKeeper::calcRelativeShapeInfo(const sead::Matrix34f&) {}
void CollisionShapeKeeper::registerCollideResult(const CollidedShapeResult&) {}
void CollisionShapeKeeper::registerCollideSupportResult(const CollidedShapeResult&) {}
bool CollisionShapeKeeper::isCollidedResultFull() const {}
bool CollisionShapeKeeper::isCollidedSupportResultFull() const {}
bool CollisionShapeKeeper::isShapeArrow(s32) const {}
bool CollisionShapeKeeper::isShapeSphere(s32) const {}
bool CollisionShapeKeeper::isShapeDisk(s32) const {}
const CollisionShapeInfoBase& CollisionShapeKeeper::getShapeInfoBase(s32) const {}
const CollisionShapeInfoArrow& CollisionShapeKeeper::getShapeInfoArrow(s32) const {}
const CollisionShapeInfoSphere& CollisionShapeKeeper::getShapeInfoSphere(s32) const {}
const CollisionShapeInfoDisk& CollisionShapeKeeper::getShapeInfoDisk(s32) const {}
u32 CollisionShapeKeeper::findShapeInfoIndex(const char*) const {}
const CollidedShapeResult& CollisionShapeKeeper::getCollidedShapeResult(s32) const {}
const CollidedShapeResult& CollisionShapeKeeper::getCollidedShapeSupportResult(s32) const {}
