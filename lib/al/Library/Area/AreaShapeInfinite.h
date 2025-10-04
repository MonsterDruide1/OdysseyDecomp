#pragma once

#include "Library/Area/AreaShape.h"

namespace al {

class AreaShapeInfinite : public AreaShape {
public:
    AreaShapeInfinite();

    bool isInVolume(const sead::Vector3f& pos) const override;
    bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* edgePoint, const sead::Vector3f& pos) const override;
    bool checkArrowCollision(sead::Vector3f* a2, sead::Vector3f* a3, const sead::Vector3f& a4,
                             const sead::Vector3f& a5) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const override;
};

}  // namespace al
