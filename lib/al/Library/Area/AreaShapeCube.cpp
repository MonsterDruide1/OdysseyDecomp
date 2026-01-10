#include "Library/Area/AreaShapeCube.h"

namespace al {

AreaShapeCube::AreaShapeCube(AreaShapeCube::OriginType originType) : mOriginType(originType) {}

bool AreaShapeCube::isInVolume(const sead::Vector3f& pos) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, pos);

    return isInLocalVolume(localPos);
}

bool AreaShapeCube::isInLocalVolume(const sead::Vector3f& pos) const {
    f32 bottom = mOriginType == OriginType::Base ?
                     0.0f :
                     (mOriginType == OriginType::Top ? -1000.0f : -500.0f);
    f32 top = mOriginType == OriginType::Base ? 1000.0f :
                                                (mOriginType == OriginType::Top ? 0.0f : 500.0f);

    sead::Vector3f min = {-500.0f, bottom, -500.0f};
    sead::Vector3f max = {500.0f, top, 500.0f};

    if ((pos.y < min.y || max.y < pos.y) || (pos.x < min.x || max.x < pos.x) ||
        (pos.z < min.z || max.z < pos.z))
        return false;

    return true;
}

}  // namespace al
