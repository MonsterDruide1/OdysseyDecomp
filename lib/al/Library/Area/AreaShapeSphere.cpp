#include "Library/Area/AreaShapeSphere.h"

#include <math/seadMathCalcCommon.h>

namespace al {

AreaShapeSphere::AreaShapeSphere() = default;

bool AreaShapeSphere::isInVolume(const sead::Vector3f& pos) const {
    sead::Vector3f baseTrans;
    calcTrans(&baseTrans);
    sead::Vector3f diff = pos - baseTrans;
    f32 radius = getScale().x * 500.0f;

    return diff.squaredLength() <= sead::Mathf::square(radius);
}

bool AreaShapeSphere::isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const {
    sead::Vector3f baseTrans;
    calcTrans(&baseTrans);
    sead::Vector3f diff = pos - baseTrans;
    f32 radius = getScale().x * 500.0f + offset;

    return diff.squaredLength() <= sead::Mathf::square(radius);
}

bool AreaShapeSphere::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                           const sead::Vector3f& pos) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, pos);
    f32 length = localPos.length();

    if (length > 0.0f)
        localPos *= 500.0f / length;

    calcWorldPos(edgePoint, localPos);

    return true;
}

bool AreaShapeSphere::checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                                          const sead::Vector3f&) const {
    return false;
}

bool AreaShapeSphere::calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const {
    boundingBox->set({-500.0f, -500.0f, -500.0f}, {500.0f, 500.0f, 500.0f});
    return true;
}

}  // namespace al
