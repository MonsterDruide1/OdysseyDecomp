#include "Library/Collision/Angle.h"

#include "Library/Math/MathUtil.h"

namespace al {

static constexpr f32 cWallAngleCos = 0.34202015f;  // cos(70°)

bool isWallPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity) {
    return calcPolygonType(normal, gravity) == PolygonType::Wall;
}

bool isFloorPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity) {
    return calcPolygonType(normal, gravity) == PolygonType::Floor;
}

bool isFloorPolygonCos(const sead::Vector3f& normal, const sead::Vector3f& gravity, f32 maxCos) {
    if (isNearZero(normal))
        return 0;

    f32 cos = normal.dot(gravity);
    if (-cos < maxCos)
        return false;

    return !(sead::Mathf::abs(cos) < cWallAngleCos) && (cos < 0.0f);
}

bool isCeilingPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity) {
    // For some reason this doesn't want to work, at least not without breaking the others
    // return calcPolygonType(normal, gravity) == PolygonType::Ceiling;

    if (isNearZero(normal))
        return 0;

    f32 cos = normal.dot(gravity);
    if (sead::Mathf::abs(cos) < cWallAngleCos)
        return false;
    else if (normal.dot(gravity) < 0.0f)
        return false;
    else
        return true;
}

PolygonType calcPolygonType(const sead::Vector3f& normal, const sead::Vector3f& gravity) {
    if (isNearZero(normal))
        return PolygonType::None;

    f32 cos = normal.dot(gravity);
    if (sead::Mathf::abs(cos) < cWallAngleCos)
        return PolygonType::Wall;
    else if (cos < 0.0f)
        return PolygonType::Floor;
    else
        return PolygonType::Ceiling;
}

void calcTriangleColorByAngle(sead::Color4f* outColor, f32* outAngle,
                              const sead::Vector3f& normal) {
    f32 angle = calcAngleDegree(normal, sead::Vector3f::ey);

    if (angle >= 60 && angle < 79.5)
        *outColor = sead::Color4f::cCyan;
    else if (angle >= 79.5 && angle <= 110)
        *outColor = sead::Color4f::cBlue;
    else if (angle > 110)
        *outColor = sead::Color4f::cMagenta;
    else
        *outColor = sead::Color4f::cGreen;

    if (outAngle)
        *outAngle = angle;
}

}  // namespace al
