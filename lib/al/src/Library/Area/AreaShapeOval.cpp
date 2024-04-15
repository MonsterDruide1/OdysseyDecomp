#include "Library/Area/AreaShapeOval.h"

#include <math/seadMathCalcCommon.h>

namespace al {

AreaShapeOval::AreaShapeOval() {}

bool AreaShapeOval::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return localPos.squaredLength() <= sead::Mathf::square(500.0f);
}

// bool AreaShapeOval::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const

bool AreaShapeOval::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                         const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);
    f32 length = localPos.length();

    if (length > 0.0f)
        localPos *= 500.0f / length;

    calcWorldPos(edgePoint, localPos);
    return true;
}

// bool AreaShapeOval::checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
// const sead::Vector3f&) const

bool AreaShapeOval::calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const {
    return false;
}

}  // namespace al
