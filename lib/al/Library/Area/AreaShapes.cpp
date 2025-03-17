#include "Library/Area/AreaShapes.h"

#include "Library/Math/MathUtil.h"

namespace al {
AreaShapeCube::AreaShapeCube(AreaShapeCube::OriginType originType) : mOriginType(originType) {}

/*bool AreaShapeCube::calcLocalBoundingBox(sead::BoundBox3f* out) const {
    out->setUndef();

    f32 bottom = calcBottom();
    out->set({-500.0f, bottom, -500.0f}, {-500.0f, bottom, -500.0f});

    f32 top = calcTop();

    if (bottom > top)
        out->setMinY(top);

    out->setMaxX(500.0f);

    if (bottom < top)
        out->setMaxY(top);

    out->setMaxZ(500.0f);

    return true;
}*/

bool AreaShapeCube::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return isInLocalVolume(localPos);
}

bool AreaShapeCube::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    f32 bottom = calcBottom();
    f32 top = calcTop();

    sead::Vector3f scale = getScale();
    if (isNearZero(scale.x, 0.001f))
        return false;
    if (isNearZero(scale.y, 0.001f))
        return false;
    if (isNearZero(scale.z, 0.001f))
        return false;

    scale.x = offset / scale.x;
    scale.y = offset / scale.y;
    scale.z = offset / scale.z;

    if (localPos.y < bottom - scale.y || top + scale.y < localPos.y)
        return false;

    if (localPos.x < -500.0f - scale.x || 500.0f + scale.x < localPos.x)
        return false;

    if (localPos.z < -500.0f - scale.z || 500.0f + scale.z < localPos.z)
        return false;

    return true;
}

// bool AreaShapeCube::calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const
// {}

bool AreaShapeCube::isInLocalVolume(const sead::Vector3f& trans) const {
    sead::Vector3f min = {-500.0f, calcBottom(), -500.0f};
    sead::Vector3f max = {500.0f, calcTop(), 500.0f};

    if ((trans.y < min.y || max.y < trans.y) || (trans.x < min.x || max.x < trans.x) ||
        (trans.z < min.z || max.z < trans.z))
        return false;
    else
        return true;
}

// bool AreaShapeCube::checkArrowCollision(sead::Vector3f* outPos, sead::Vector3f* outNormal,
//                                         const sead::Vector3f& pos1,
//                                         const sead::Vector3f& pos2) const {}

AreaShapeSphere::AreaShapeSphere() = default;

bool AreaShapeSphere::calcLocalBoundingBox(sead::BoundBox3f* out) const {
    out->set({-500.0f, -500.0f, -500.0f}, {500.0f, 500.0f, 500.0f});

    return true;
}

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

bool AreaShapeSphere::calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);
    f32 length = localPos.length();

    if (length > 0.0f)
        localPos *= 500.0f / length;

    calcWorldPos(out, localPos);

    return true;
}

bool AreaShapeSphere::checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                                          const sead::Vector3f& pos1,
                                          const sead::Vector3f& pos2) const {
    return false;
}

AreaShapeOval::AreaShapeOval() = default;

bool AreaShapeOval::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return localPos.squaredLength() <= sead::Mathf::square(500.0f);
}

bool AreaShapeOval::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    sead::Vector3f scale = getScale();

    // has to be this way around to match
    if (scale.x == scale.y && scale.y == scale.z) {
        sead::Vector3f baseTrans;
        calcTrans(&baseTrans);
        sead::Vector3f offsetTrans = trans - baseTrans;
        f32 radius = scale.x * 500.0f + offset;

        return offsetTrans.squaredLength() <= sead::Mathf::square(radius);
    }

    return false;
}

bool AreaShapeOval::calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);
    f32 length = localPos.length();

    if (length > 0.0f)
        localPos *= 500.0f / length;

    calcWorldPos(out, localPos);

    return true;
}

bool AreaShapeOval::checkArrowCollision(sead::Vector3f* outPos, sead::Vector3f* outNormal,
                                        const sead::Vector3f& pos1,
                                        const sead::Vector3f& pos2) const {
    sead::Vector3f localPos1 = sead::Vector3f::zero;
    calcLocalPos(&localPos1, pos1);
    sead::Vector3f localPos2 = sead::Vector3f::zero;
    calcLocalPos(&localPos2, pos2);

    sead::Vector3f worldPos1 = {0, 0, 0};
    sead::Vector3f worldPos2 = {0, 0, 0};
    calcWorldPos(&worldPos1, localPos1);
    calcWorldPos(&worldPos2, localPos2);

    if (!checkHitSegmentSphere(sead::Vector3f::zero, localPos1, localPos2, 500.0f, outNormal,
                               outPos))
        return false;

    calcWorldPos(outPos, *outPos);
    calcWorldDir(outNormal, *outNormal);
    *outNormal *= -1.0f;

    return true;
}

AreaShapeCylinder::AreaShapeCylinder(AreaShapeCylinder::OriginType originType)
    : mOriginType(originType) {}

/*bool AreaShapeCylinder::calcLocalBoundingBox(sead::BoundBox3f* out) const {
    out->setUndef();

    f32 bottom = calcBottom();
    out->set({-500.0f, bottom, -500.0f}, {-500.0f, bottom, -500.0f});

    f32 top = calcTop();

    if (bottom > top)
        out->setMinY(top);

    out->setMaxX(500.0f);

    if (bottom < top)
        out->setMaxY(top);

    out->setMaxZ(500.0f);

    return true;
}*/

bool AreaShapeCylinder::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    f32 bottom = calcBottom();
    f32 top = calcTop();

    if (localPos.y < bottom || top < localPos.y)
        return false;

    return sead::Mathf::square(localPos.x) + sead::Mathf::square(localPos.z) <=
           sead::Mathf::square(500.0f);
}

bool AreaShapeCylinder::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    sead::Vector3f scale = getScale();
    if (isNearZero(scale.x, 0.001f))
        return false;
    if (isNearZero(scale.y, 0.001f))
        return false;
    if (isNearZero(scale.z, 0.001f))
        return false;

    scale.y = offset / scale.y;

    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    f32 bottom = calcBottom();
    f32 top = calcTop();

    scale.x = offset / scale.x + 500.0f;
    if (localPos.y < bottom - scale.y || scale.y + top < localPos.y)
        return false;

    return sead::Mathf::square(localPos.x) + sead::Mathf::square(localPos.z) <=
           sead::Mathf::square(scale.x);
}

// bool AreaShapeCylinder::calcNearestEdgePoint(sead::Vector3f* out,
//                                              const sead::Vector3f& trans) const {}

bool AreaShapeCylinder::checkArrowCollision(sead::Vector3f* outPos, sead::Vector3f* outNormal,
                                            const sead::Vector3f& pos1,
                                            const sead::Vector3f& pos2) const {
    sead::Vector3f localPos1 = sead::Vector3f::zero;
    calcLocalPos(&localPos1, pos1);
    sead::Vector3f localPos2 = sead::Vector3f::zero;
    calcLocalPos(&localPos2, pos2);

    sead::Vector3f bottomPos = sead::Vector3f::zero;
    bottomPos.y = calcBottom();
    if (!checkHitSegmentCylinder(bottomPos, 500.0f, sead::Vector3f::ey, 500.0f, localPos1,
                                 localPos2, outPos, outNormal))
        return false;

    if (outPos)
        calcWorldPos(outPos, *outPos);
    if (outNormal)
        calcWorldDir(outNormal, *outNormal);

    return true;
}

AreaShapeInfinite::AreaShapeInfinite() = default;

bool AreaShapeInfinite::isInVolume(const sead::Vector3f& trans) const {
    return true;
}

bool AreaShapeInfinite::isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const {
    return true;
}

bool AreaShapeInfinite::calcNearestEdgePoint(sead::Vector3f* out,
                                             const sead::Vector3f& trans) const {
    return false;
}

bool AreaShapeInfinite::checkArrowCollision(sead::Vector3f* outPos, sead::Vector3f* outNormal,
                                            const sead::Vector3f& pos1,
                                            const sead::Vector3f& pos2) const {
    return false;
}

bool AreaShapeInfinite::calcLocalBoundingBox(sead::BoundBox3f* out) const {
    return false;
}

bool AreaShapeOval::calcLocalBoundingBox(sead::BoundBox3f* out) const {
    return false;
}
}  // namespace al
