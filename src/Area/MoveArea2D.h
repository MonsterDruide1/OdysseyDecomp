#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class MoveArea2D : public al::AreaObj {
public:
    enum class ShapeType {
        None = 0,
        CubeBase = 1,
        Cylinder = 2,
        CylinderCenter = 3,
        Disk = 4,
    };

    enum class SnapGravityType {
        Default = 0,
        Vertical = 1,
        Radial = 2,
    };

    MoveArea2D(const char* name);

    void init(const al::AreaInitInfo& info) override;

    bool calcGravityCylinderCenterAxis(sead::Vector3f* dirH, f32* outDist,
                                       const sead::Vector3f& pos, bool isReverse) const;
    bool calcGravityDir(sead::Vector3f* outDir, f32* outDist, const sead::Vector3f& pos) const;
    bool calcGravityYDir(sead::Vector3f* outDir, f32* outDist) const;

    bool calcSnapPower(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                       f32 surfaceDistance) const;
    bool calcSnapPowerCube(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                           f32 surfaceDistance) const;
    bool calcSnapPowerCylinder(sead::Vector3f* dirH, f32* outPower, const sead::Vector3f& pos,
                               f32 surfaceDistance) const;
    bool calcSnapPowerDisk(sead::Vector3f* outDir, f32* outPower, const sead::Vector3f& pos,
                           f32 surfaceDistance) const;

private:
    ShapeType mShapeType = ShapeType::None;
    f32 mSurfaceDistance = 0.0f;
    f32 mGravityOffset = 0.0f;
};
