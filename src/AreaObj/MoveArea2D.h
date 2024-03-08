#include "al/include/Library/Area/AreaObj.h"
#include "math/seadVectorFwd.h"

class MoveArea2D : public al::AreaObj {
private:
    s32 mModelIndex;
    f32 mSurfaceDistance;
    f32 mGravityOffset;

public:
    MoveArea2D(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

    bool calcGravityCylinderCenterAxis(sead::Vector3f*, f32*, const sead::Vector3f&, bool) const;
    bool calcGravityDir(sead::Vector3f*, f32*, const sead::Vector3f&) const;
    bool calcGravityYDir(sead::Vector3f*, f32*) const;

    bool calcSnapPower(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerCube(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerCylinder(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
    bool calcSnapPowerDisk(sead::Vector3f*, f32*, const sead::Vector3f&, f32);
};
