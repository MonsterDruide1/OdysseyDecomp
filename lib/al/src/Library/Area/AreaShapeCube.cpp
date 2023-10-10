#include "Library/Area/AreaShapeCube.h"

namespace al {

AreaShapeCube::AreaShapeCube(AreaShapeCube::OriginType originType) : mOriginType(originType) {}

bool AreaShapeCube::isInVolume(const sead::Vector3f& trans) const {
    sead::Vector3f localPos = sead::Vector3f::zero;
    calcLocalPos(&localPos, trans);

    return isInLocalVolume(localPos);
}

bool AreaShapeCube::isInLocalVolume(const sead::Vector3f& trans) const {
    float bottom = mOriginType == OriginType::Base ?
                       0.0f :
                       (mOriginType == OriginType::Top ? -1000.0f : 500.0f);
    float top = mOriginType == OriginType::Base ? 1000.0f :
                                                  (mOriginType == OriginType::Top ? 0.0f : 500.0f);

    sead::Vector3f min = {-500.0f, bottom, -500.0f};
    sead::Vector3f max = {500.0f, top, 500.0f};

    if ((trans.y < min.y || max.y < trans.y) || (trans.x < min.x || max.x < trans.x) ||
        (trans.z < min.z || max.z < trans.z))
        return false;

    return true;
}

}  // namespace al
