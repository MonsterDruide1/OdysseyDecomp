#pragma once

#include <math/seadVector.h>

class CollisionShapeInfoBase;
class CollisionShapeInfoArrow;
class CollisionShapeInfoDisk;
class CollisionShapeInfoSphere;

namespace CollisionShapeFunction {

bool isShapeName(const CollisionShapeInfoBase* shape, const char* name);

bool isShapeArrow(const CollisionShapeInfoBase* shape);
bool isShapeSphere(const CollisionShapeInfoBase* shape);
bool isShapeDisk(const CollisionShapeInfoBase* shape);

CollisionShapeInfoArrow* getShapeInfoArrow(CollisionShapeInfoBase* shape);
CollisionShapeInfoSphere* getShapeInfoSphere(CollisionShapeInfoBase* shape);
CollisionShapeInfoDisk* getShapeInfoDisk(CollisionShapeInfoBase* shape);
const CollisionShapeInfoArrow* getShapeInfoArrow(const CollisionShapeInfoBase* shape);
const CollisionShapeInfoSphere* getShapeInfoSphere(const CollisionShapeInfoBase* shape);
const CollisionShapeInfoDisk* getShapeInfoDisk(const CollisionShapeInfoBase* shape);

void updateShapeOffset(CollisionShapeInfoBase* shape, const sead::Vector3f& offset);

}  // namespace CollisionShapeFunction
