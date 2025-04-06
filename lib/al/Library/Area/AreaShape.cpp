#include "Library/Area/AreaShape.h"

#include <math/seadVector.h>

#include "Library/Area/AreaShapeCube.h"
#include "Library/Area/AreaShapeCylinder.h"
#include "Library/Area/AreaShapeInfinite.h"
#include "Library/Area/AreaShapeSphere.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {

AreaShape::AreaShape() {}

void AreaShape::setBaseMtxPtr(const sead::Matrix34f* baseMtxPtr) {
    mBaseMtxPtr = baseMtxPtr;
}

void AreaShape::setScale(const sead::Vector3f& scale) {
    mScale = scale;
}

bool AreaShape::calcLocalPos(sead::Vector3f* localPos, const sead::Vector3f& trans) const {
    if (isNearZeroOrLess(mScale.x, 0.001))
        return false;
    if (isNearZeroOrLess(mScale.y, 0.001))
        return false;
    if (isNearZeroOrLess(mScale.z, 0.001))
        return false;

    if (mBaseMtxPtr)
        calcMtxLocalTrans(localPos, *mBaseMtxPtr, trans);
    else
        localPos->e = trans.e;

    f32 localX = localPos->x;
    localPos->x = localX / mScale.x;
    f32 localY = localPos->y;
    localPos->y = localY / mScale.y;
    f32 localZ = localPos->z;
    localPos->z = localZ / mScale.z;

    return true;
}

bool AreaShape::calcWorldPos(sead::Vector3f* worldPos, const sead::Vector3f& trans) const {
    if (isNearZero(mScale.x))
        return false;
    if (isNearZero(mScale.y))
        return false;
    if (isNearZero(mScale.z))
        return false;

    worldPos->x = trans.x * mScale.x;
    worldPos->y = trans.y * mScale.y;
    worldPos->z = trans.z * mScale.z;

    if (mBaseMtxPtr)
        worldPos->setMul(*mBaseMtxPtr, *worldPos);

    return true;
}

bool AreaShape::calcWorldDir(sead::Vector3f* worldDir, const sead::Vector3f& trans) const {
    if (isNearZero(mScale.x))
        return false;
    if (isNearZero(mScale.y))
        return false;
    if (isNearZero(mScale.z))
        return false;

    worldDir->x = trans.x * mScale.x;
    worldDir->y = trans.y * mScale.y;
    worldDir->z = trans.z * mScale.z;

    if (mBaseMtxPtr)
        worldDir->setRotated(*mBaseMtxPtr, *worldDir);

    tryNormalizeOrZero(worldDir);

    return true;
}

void AreaShape::calcTrans(sead::Vector3f* trans) const {
    if (mBaseMtxPtr)
        mBaseMtxPtr->getTranslation(*trans);
    else
        trans->e = sead::Vector3f::zero.e;
}

template <typename T>
AreaShape* createAreaShapeFunction() {
    return new T;
}

static NameToCreator<AreaShapeCreatorFunction> sAreaShapeEntries[] = {
    {"AreaCubeBase", *createAreaShapeFunction<AreaShapeCubeBase>},
    {"AreaCubeCenter", createAreaShapeFunction<AreaShapeCubeCenter>},
    {"AreaCubeTop", createAreaShapeFunction<AreaShapeCubeTop>},
    {"AreaSphere", createAreaShapeFunction<AreaShapeSphere>},
    {"AreaCylinder", createAreaShapeFunction<AreaShapeCylinderBase>},
    {"AreaCylinderCenter", createAreaShapeFunction<AreaShapeCylinderCenter>},
    {"AreaCylinderTop", createAreaShapeFunction<AreaShapeCylinderTop>},
    {"AreaInfinite", createAreaShapeFunction<AreaShapeInfinite>},
};

AreaShapeFactory::AreaShapeFactory(const char* factoryName)
    : Factory<AreaShape* (*)()>(factoryName, sAreaShapeEntries) {}

}  // namespace al
