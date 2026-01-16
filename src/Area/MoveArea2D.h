#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class MoveArea2D : public al::AreaObj {
public:
    enum class ShapeType { Cube = 1, Cylinder, CylinderCenter, Disk };

    MoveArea2D(const char* name);

    void init(const al::AreaInitInfo& info) override;

    bool calcGravityCylinderCenterAxis(sead::Vector3f*, f32*, const sead::Vector3f&, bool) const;
    bool calcGravityDir(sead::Vector3f*, f32*, const sead::Vector3f&) const;
    bool calcGravityYDir(sead::Vector3f*, f32*) const;

    bool calcSnapPower(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerCube(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerCylinder(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerDisk(sead::Vector3f*, f32*, const sead::Vector3f&, f32);

private:
    ShapeType mShapeType;
    f32 mSurfaceDistance;
    f32 mGravityOffset;
};
