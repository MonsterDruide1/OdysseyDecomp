#include "Area/MoveArea2D.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

MoveArea2D::MoveArea2D(const char* name) : al::AreaObj(name) {}

void MoveArea2D::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);
    al::tryGetAreaObjArg(&mSurfaceDistance, this, "SurfaceDistance");
    al::tryGetAreaObjArg(&mGravityOffset, this, "GravityOffset");

    const char* shapeName = nullptr;
    alPlacementFunction::tryGetModelName(&shapeName, info);

    SnapGravityType gravityType = SnapGravityType::Default;
    al::tryGetAreaObjArg((s32*)&gravityType, this, "SnapGravityType");

    if (al::isEqualString(shapeName, "AreaCubeBase")) {
        mShapeType = ShapeType::CubeBase;
        return;
    } else if (al::isEqualString(shapeName, "AreaCylinder")) {
        mShapeType = ShapeType::Cylinder;
        return;
    } else if (al::isEqualString(shapeName, "AreaCylinderCenter")) {
        switch (gravityType) {
        case SnapGravityType::Default:
        case SnapGravityType::Radial:
            mShapeType = ShapeType::Disk;
            return;
        case SnapGravityType::Vertical:
            mShapeType = ShapeType::CylinderCenter;
            return;
        default:
            mShapeType = ShapeType::Disk;
            return;
        }
    }

    invalidate();
}

bool MoveArea2D::calcSnapPower(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                               f32 surfaceDistance) const {
    switch (mShapeType) {
    case ShapeType::CubeBase:
        return calcSnapPowerCube(outDir, outPower, pos, surfaceDistance);
    case ShapeType::Cylinder:
        return calcSnapPowerCylinder(outDir, outPower, pos, surfaceDistance);
    case ShapeType::CylinderCenter:
        return calcSnapPowerDisk(outDir, outPower, pos, surfaceDistance);
    case ShapeType::Disk:
        return calcSnapPowerDisk(outDir, outPower, pos, surfaceDistance);
    default:
        return false;
    }
}

bool MoveArea2D::calcSnapPowerDisk(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                                   f32 surfaceDistance) const {
    const sead::Matrix34f& mtx = getAreaMtx();
    sead::Vector3f axis = mtx.getBase(1);
    sead::Vector3f diff = mtx.getTranslation() - pos;

    f32 distance = axis.dot(diff);

    f32 power;
    if (al::isNearZeroOrLess(distance)) {
        *outDir = -axis;
        power = sead::Mathf::abs(distance) - surfaceDistance;
    } else {
        *outDir = axis;
        power = sead::Mathf::abs(distance) + surfaceDistance;
    }

    if (!al::isNearZeroOrGreater(power)) {
        outDir->negate();
        power = -power;
    }

    *outPower = power;
    return true;
}

bool MoveArea2D::calcSnapPowerCube(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                                   f32 surfaceDistance) const {
    const sead::Matrix34f& mtx = getAreaMtx();
    sead::Vector3f axis = mtx.getBase(2);
    sead::Vector3f diff = mtx.getTranslation() - pos;

    f32 distance = axis.dot(diff);

    f32 power;
    if (al::isNearZeroOrLess(distance)) {
        *outDir = -axis;
        power = sead::Mathf::abs(distance) - surfaceDistance;
    } else {
        *outDir = axis;
        power = sead::Mathf::abs(distance) + surfaceDistance;
    }

    if (!al::isNearZeroOrGreater(power)) {
        outDir->negate();
        power = -power;
    }

    *outPower = power;
    return true;
}

bool MoveArea2D::calcSnapPowerCylinder(sead::Vector3f* dirH, f32* outPower,
                                       const sead::Vector3f& pos, f32 surfaceDistance) const {
    const sead::Matrix34f& mtx = getAreaMtx();
    sead::Vector3f offH = mtx.getTranslation();
    offH -= pos;
    sead::Vector3f upAxis;
    mtx.getBase(upAxis, 1);

    al::verticalizeVec(&offH, upAxis, offH);

    if (!al::tryNormalizeOrZero(dirH, offH))
        return false;

    f32 delta = offH.length() - surfaceDistance;

    if (!al::isNearZeroOrGreater(delta)) {
        dirH->negate();
        delta = -delta;
    }

    *outPower = delta;
    return true;
}

bool MoveArea2D::calcGravityDir(sead::Vector3f* outDir, f32* outDist,
                                const sead::Vector3f& pos) const {
    switch (mShapeType) {
    case ShapeType::CubeBase:
        return calcGravityYDir(outDir, outDist);
    case ShapeType::Cylinder:
        return calcGravityYDir(outDir, outDist);
    case ShapeType::CylinderCenter:
        return calcGravityCylinderCenterAxis(outDir, outDist, pos, false);
    case ShapeType::Disk:
        return calcGravityCylinderCenterAxis(outDir, outDist, pos, true);
    default:
        return false;
    }
}

bool MoveArea2D::calcGravityYDir(sead::Vector3f* outDir, f32* outDist) const {
    const sead::Matrix34f& mtx = getAreaMtx();
    mtx.getBase(*outDir, 1);
    outDir->negate();
    *outDist = 1.0f;
    return true;
}

bool MoveArea2D::calcGravityCylinderCenterAxis(sead::Vector3f* dirH, f32* outDist,
                                               const sead::Vector3f& pos, bool isReverse) const {
    const sead::Matrix34f& mtx = getAreaMtx();
    sead::Vector3f offH = mtx.getTranslation();
    offH -= pos;

    *outDist = offH.length();

    sead::Vector3f upAxis;
    mtx.getBase(upAxis, 1);
    al::verticalizeVec(&offH, upAxis, offH);

    if (!al::tryNormalizeOrZero(dirH, offH)) {
        *outDist = 0.0f;
        *dirH = {0.0f, 0.0f, 0.0f};
        return false;
    }

    if (isReverse)
        dirH->negate();

    return true;
}
