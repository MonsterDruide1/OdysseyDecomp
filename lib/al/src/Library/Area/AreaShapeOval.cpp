#include "Library/Area/AreaShapeOval.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/SegmentUtil.h"

namespace al {

AreaShapeOval::AreaShapeOval() {}

bool AreaShapeOval::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return localPos.squaredLength() <= sead::Mathf::square(500.0f);
}

// NON_MATCHING: comparison at the top with different codegen
bool AreaShapeOval::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    float scaleX = getScale().x;
    if (scaleX != getScale().y || scaleX != getScale().z)
        return false;

    sead::Vector3f baseTrans;
    calcTrans(&baseTrans);
    sead::Vector3f offsetTrans = trans - baseTrans;
    f32 radius = scaleX * 500.0f + offset;

    return offsetTrans.squaredLength() <= sead::Mathf::square(radius);
}

bool AreaShapeOval::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                         const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);
    float length = localPos.length();

    if (length > 0.0f)
        localPos *= 500.0f / length;

    calcWorldPos(edgePoint, localPos);
    return true;
}

bool AreaShapeOval::checkArrowCollision(sead::Vector3f* a2, sead::Vector3f* a3,
                                        const sead::Vector3f& a4, const sead::Vector3f& a5) const {
    sead::Vector3f localA4 = sead::Vector3f::zero;
    calcLocalPos(&localA4, a4);
    sead::Vector3f localA5 = sead::Vector3f::zero;
    calcLocalPos(&localA5, a5);

    sead::Vector3f tmp1 = {0, 0, 0};
    sead::Vector3f tmp2 = {0, 0, 0};
    calcWorldPos(&tmp1, localA4);
    calcWorldPos(&tmp2, localA5);

    if (!checkHitSegmentSphere(sead::Vector3f::zero, localA4, localA5, 500.0f, a3, a2))
        return false;

    calcWorldPos(a2, *a2);
    calcWorldDir(a3, *a3);
    *a3 *= (-1);
    return true;
}

bool AreaShapeOval::calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const {
    return false;
}

}  // namespace al
