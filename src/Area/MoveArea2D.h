#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class MoveArea2D : public al::AreaObj {
public:
    enum class ShapeType {
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

    bool calcGravityCylinderCenterAxis(sead::Vector3f*, f32*, const sead::Vector3f&, bool) const;
    bool calcGravityDir(sead::Vector3f*, f32*, const sead::Vector3f&) const;
    bool calcGravityYDir(sead::Vector3f*, f32*) const;

    bool calcSnapPower(sead::Vector3f*, f32*, const sead::Vector3f&, f32) const;
    bool calcSnapPowerCube(sead::Vector3f*, f32*, const sead::Vector3f&, f32) const;
    bool calcSnapPowerCylinder(sead::Vector3f*, f32*, const sead::Vector3f&, f32) const;
    bool calcSnapPowerDisk(sead::Vector3f*, f32*, const sead::Vector3f&, f32) const;

private:
    ShapeType mShapeType = (ShapeType)0;
    f32 mSurfaceDistance = 0.0f;
    f32 mGravityOffset = 0.0f;
};
