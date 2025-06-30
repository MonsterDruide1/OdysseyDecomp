#pragma once

#include "Library/Area/AreaShape.h"

namespace al {

class AreaShapeSphere : public AreaShape {
public:
    AreaShapeSphere();

    bool isInVolume(const sead::Vector3f& pos) const override;
    bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* edgePoint, const sead::Vector3f& pos) const override;
    bool checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                             const sead::Vector3f&) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const override;
};

}  // namespace al
