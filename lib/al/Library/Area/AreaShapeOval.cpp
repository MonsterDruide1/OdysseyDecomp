#include "Library/Area/AreaShapeOval.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"

namespace al {

AreaShapeOval::AreaShapeOval() {}

bool AreaShapeOval::isInVolume(const sead::Vector3f& pos) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, pos);

    return localPos.squaredLength() <= sead::Mathf::square(500.0f);
}

bool AreaShapeOval::isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const {
    sead::Vector3f scale = getScale();

    // has to be this way around to match
    if (scale.x == scale.y && scale.y == scale.z) {
        sead::Vector3f baseTrans;
        calcTrans(&baseTrans);
        sead::Vector3f diff = pos - baseTrans;
        f32 radius = scale.x * 500.0f + offset;

        return diff.squaredLength() <= sead::Mathf::square(radius);
    }

    return false;
}

bool AreaShapeOval::calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                         const sead::Vector3f& pos) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, pos);
    f32 length = localPos.length();

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

}  // namespace al
