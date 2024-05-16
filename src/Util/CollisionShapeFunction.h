#pragma once

#include <math/seadVector.h>

class CollisionShapeInfoBase;
class CollisionShapeInfoArrow;
class CollisionShapeInfoDisk;
class CollisionShapeInfoSphere;

class CollisionShapeFunction {
public:
    static bool isShapeName(const CollisionShapeInfoBase*, const char*);

    static bool isShapeArrow(const CollisionShapeInfoBase*);
    static bool isShapeSphere(const CollisionShapeInfoBase*);
    static bool isShapeDisk(const CollisionShapeInfoBase*);

    static CollisionShapeInfoArrow* getShapeInfoArrow(CollisionShapeInfoBase*);
    static CollisionShapeInfoSphere* getShapeInfoSphere(CollisionShapeInfoBase*);
    static CollisionShapeInfoDisk* getShapeInfoDisk(CollisionShapeInfoBase*);
    static const CollisionShapeInfoArrow* getShapeInfoArrow(const CollisionShapeInfoBase*);
    static const CollisionShapeInfoSphere* getShapeInfoSphere(const CollisionShapeInfoBase*);
    static const CollisionShapeInfoDisk* getShapeInfoDisk(const CollisionShapeInfoBase*);

    static void updateShapeOffset(CollisionShapeInfoBase*, const sead::Vector3f&);
};
