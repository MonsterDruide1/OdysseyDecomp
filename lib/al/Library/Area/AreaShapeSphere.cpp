#include "Library/Area/AreaShapeSphere.h"

#include <math/seadMathCalcCommon.h>

namespace al {

AreaShapeSphere::AreaShapeSphere() {}

bool AreaShapeSphere::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f baseTrans;
    calcTrans(&baseTrans);
    sead::Vector3f offsetTrans = trans - baseTrans;
    f32 radius = getScale().x * 500.0f;

    return offsetTrans.squaredLength() <= sead::Mathf::square(radius);
}

bool AreaShapeSphere::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    sead::Vector3f baseTrans;
    calcTrans(&baseTrans);
    sead::Vector3f offsetTrans = trans - baseTrans;
    f32 radius = getScale().x * 500.0f + offset;

    return offsetTrans.squaredLength() <= sead::Mathf::square(radius);
}

bool AreaShapeSphere::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                           const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);
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
