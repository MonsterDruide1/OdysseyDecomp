#pragma once

#include "Library/Area/AreaShape.h"

namespace al {

class AreaShapeCylinder : public AreaShape {
public:
    enum class OriginType { Center, Base, Top };

    AreaShapeCylinder(AreaShapeCylinder::OriginType);

    bool isInVolume(const sead::Vector3f&) const override;
    bool isInVolumeOffset(const sead::Vector3f&, f32) const override;
    bool calcNearestEdgePoint(sead::Vector3f*, const sead::Vector3f&) const override;
    bool checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                             const sead::Vector3f&) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f*) const override;

private:
    AreaShapeCylinder::OriginType mOriginType;
};

class AreaShapeCylinderBase : public AreaShapeCylinder {
public:
    AreaShapeCylinderBase() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Base) {}
};

class AreaShapeCylinderCenter : public AreaShapeCylinder {
public:
    AreaShapeCylinderCenter() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Center) {}
};

class AreaShapeCylinderTop : public AreaShapeCylinder {
public:
    AreaShapeCylinderTop() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Top) {}
};

}  // namespace al
