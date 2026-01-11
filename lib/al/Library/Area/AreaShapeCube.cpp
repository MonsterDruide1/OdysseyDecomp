#include "Library/Area/AreaShapeCube.h"

namespace al {

AreaShapeCube::AreaShapeCube(AreaShapeCube::OriginType originType) : mOriginType(originType) {}

bool AreaShapeCube::isInVolume(const sead::Vector3f& pos) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, pos);

    return isInLocalVolume(localPos);
}

inline f32 getBottom(AreaShapeCube::OriginType originType) {
    switch (originType) {
    case AreaShapeCube::OriginType::Base:
        return 0.0f;
    case AreaShapeCube::OriginType::Top:
        return -1000.0f;
    default:  // AreaShapeCube::OriginType::Center
        return -500.0f;
    }
}

inline f32 getTop(AreaShapeCube::OriginType originType) {
    switch (originType) {
    case AreaShapeCube::OriginType::Base:
        return 1000.0f;
    case AreaShapeCube::OriginType::Top:
        return 0.0f;
    default:  // AreaShapeCube::OriginType::Center
        return 500.0f;
    }
}

bool AreaShapeCube::isInLocalVolume(const sead::Vector3f& pos) const {
    sead::Vector3f min = {-500.0f, getBottom(mOriginType), -500.0f};
    sead::Vector3f max = {500.0f, getTop(mOriginType), 500.0f};

    if ((pos.y < min.y || max.y < pos.y) || (pos.x < min.x || max.x < pos.x) ||
        (pos.z < min.z || max.z < pos.z))
        return false;

    return true;
}

}  // namespace al
