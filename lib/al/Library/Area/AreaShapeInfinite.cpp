#include "Library/Area/AreaShapeInfinite.h"

namespace al {

AreaShapeInfinite::AreaShapeInfinite() = default;

bool AreaShapeInfinite::isInVolume(const sead::Vector3f& pos) const {
    return true;
}

bool AreaShapeInfinite::isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const {
    return true;
}

bool AreaShapeInfinite::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                             const sead::Vector3f& pos) const {
    return false;
}

bool AreaShapeInfinite::checkArrowCollision(sead::Vector3f* a2, sead::Vector3f* a3,
                                            const sead::Vector3f& a4,
                                            const sead::Vector3f& a5) const {
    return false;
}

bool AreaShapeInfinite::calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const {
    return false;
}

}  // namespace al
