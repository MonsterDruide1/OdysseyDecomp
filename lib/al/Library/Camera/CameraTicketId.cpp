#include "Library/Camera/CameraTicketId.h"

#include "Library/Base/StringUtil.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraTicketId::CameraTicketId(const PlacementId* placementId, const char* suffix)
    : mPlacementId(placementId), mSuffix(suffix) {}

bool CameraTicketId::isEqual(const CameraTicketId& other) const {
    if (!mPlacementId && other.mPlacementId)
        return false;
    if (mPlacementId && !other.mPlacementId)
        return false;

    if (!mPlacementId && !other.mPlacementId)
        return isEqualString(mSuffix, other.mSuffix);
    if (!mPlacementId->isEqual(*other.mPlacementId))
        return false;

    bool noneSuffix = !mSuffix && !other.mSuffix;
    if (mSuffix && other.mSuffix)
        return isEqualString(mSuffix, other.mSuffix);

    return noneSuffix;
}

bool CameraTicketId::isEqual(const CameraTicketId& ticket1, const CameraTicketId& ticket2) {
    return ticket1.isEqual(ticket2);
}

// NON_MATCHING: https://decomp.me/scratch/4vdbx
bool CameraTicketId::isEqual(const ByamlIter& iter) const {
    const char* id = getObjId();
    const char* otherId = tryGetByamlKeyStringOrNULL(iter, "ObjId");
    if (id && otherId) {
        if (!isEqualString(id, otherId))
            return false;
    } else if (id || otherId)
        return false;

    const char* suffix = mSuffix;
    const char* otherSuffix = tryGetByamlKeyStringOrNULL(iter, "Suffix");
    if (suffix && otherSuffix) {
        if (!isEqualString(suffix, otherSuffix))
            return false;
    } else
        return !suffix && !otherSuffix;
    return true;
}

const char* CameraTicketId::tryGetObjId() const {
    if (!mPlacementId)
        return nullptr;
    return mPlacementId->getId();
}

const char* CameraTicketId::getObjId() const {
    return tryGetObjId();
}

}  // namespace al
