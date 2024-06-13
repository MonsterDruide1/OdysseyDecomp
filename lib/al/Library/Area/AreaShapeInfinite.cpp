#include "Library/Area/AreaShapeInfinite.h"

namespace al {

AreaShapeInfinite::AreaShapeInfinite() {}

bool AreaShapeInfinite::isInVolume(const sead::Vector3f&) const {
    return true;
}

bool AreaShapeInfinite::isInVolumeOffset(const sead::Vector3f&, f32) const {
    return true;
}

bool AreaShapeInfinite::calcNearestEdgePoint(sead::Vector3f*, const sead::Vector3f&) const {
    return false;
}

bool AreaShapeInfinite::checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                                            const sead::Vector3f&) const {
    return false;
}

bool AreaShapeInfinite::calcLocalBoundingBox(sead::BoundBox3f*) const {
    return false;
}

}  // namespace al
