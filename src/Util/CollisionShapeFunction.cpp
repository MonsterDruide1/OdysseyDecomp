#include "Util/CollisionShapeFunction.h"

#include "Library/Base/String.h"

#include "Player/CollisionShapeInfo.h"

bool CollisionShapeFunction::isShapeName(const CollisionShapeInfoBase* shape, const char* name) {
    return al::isEqualString(name, shape->getName());
}

bool CollisionShapeFunction::isShapeArrow(const CollisionShapeInfoBase* shape) {
    return shape->getId() == CollisionShapeId::Arrow;
}
bool CollisionShapeFunction::isShapeSphere(const CollisionShapeInfoBase* shape) {
    return shape->getId() == CollisionShapeId::Sphere;
}
bool CollisionShapeFunction::isShapeDisk(const CollisionShapeInfoBase* shape) {
    return shape->getId() == CollisionShapeId::Disk;
}

CollisionShapeInfoArrow* CollisionShapeFunction::getShapeInfoArrow(CollisionShapeInfoBase* shape) {
    return reinterpret_cast<CollisionShapeInfoArrow*>(shape);
}
CollisionShapeInfoSphere* CollisionShapeFunction::getShapeInfoSphere(CollisionShapeInfoBase* shape) {
    return reinterpret_cast<CollisionShapeInfoSphere*>(shape);
}
CollisionShapeInfoDisk* CollisionShapeFunction::getShapeInfoDisk(CollisionShapeInfoBase* shape) {
    return reinterpret_cast<CollisionShapeInfoDisk*>(shape);
}
const CollisionShapeInfoArrow*
CollisionShapeFunction::getShapeInfoArrow(const CollisionShapeInfoBase* shape) {
    return reinterpret_cast<const CollisionShapeInfoArrow*>(shape);
}
const CollisionShapeInfoSphere*
CollisionShapeFunction::getShapeInfoSphere(const CollisionShapeInfoBase* shape) {
    return reinterpret_cast<const CollisionShapeInfoSphere*>(shape);
}
const CollisionShapeInfoDisk*
CollisionShapeFunction::getShapeInfoDisk(const CollisionShapeInfoBase* shape) {
    return reinterpret_cast<const CollisionShapeInfoDisk*>(shape);
}

void CollisionShapeFunction::updateShapeOffset(CollisionShapeInfoBase* shape, const sead::Vector3f& offset) {
    shape->updateShapeOffset(offset);
}
