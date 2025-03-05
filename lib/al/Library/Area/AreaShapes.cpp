#include "Library/Area/AreaShapes.h"

namespace al {
AreaShapeCube::AreaShapeCube(AreaShapeCube::OriginType originType) : mOriginType(originType) {}

// bool AreaShapeCube::calcLocalBoundingBox(sead::BoundBox3f*) const {}

bool AreaShapeCube::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return isInLocalVolume(localPos);
}

// bool AreaShapeCube::isInVolumeOffset(const sead::Vector3f&, f32) const {}
// bool AreaShapeCube::calcNearestEdgePoint(sead::Vector3f*, const sead::Vector3f&) const {}

bool AreaShapeCube::isInLocalVolume(const sead::Vector3f& trans) const {
    sead::Vector3f min = {-500.0f, calcBottom(), -500.0f};
    sead::Vector3f max = {500.0f, calcTop(), 500.0f};

    if ((trans.y < min.y || max.y < trans.y) || (trans.x < min.x || max.x < trans.x) ||
        (trans.z < min.z || max.z < trans.z))
        return false;
    else
        return true;
}

// bool AreaShapeCube::checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
//                                         const sead::Vector3f&) const {}
}  // namespace al
