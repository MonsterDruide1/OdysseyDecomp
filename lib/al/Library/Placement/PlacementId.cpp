#include "Library/Placement/PlacementId.h"

#include "Library/Base/StringUtil.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {

PlacementId::PlacementId() {}

PlacementId::PlacementId(const char* id, const char* unitConfig, const char* zoneId)
    : mId(id), mUnitConfigName(unitConfig), mZoneId(zoneId) {}

bool PlacementId::init(const PlacementInfo& info) {
    mId = nullptr;
    mZoneId = nullptr;
    mUnitConfigName = nullptr;
    mCommonId = nullptr;

    info.getPlacementIter().tryGetStringByKey(&mCommonId, "CommonId");
    info.getZoneIter().tryGetStringByKey(&mUnitConfigName, "UnitConfigName");
    info.getZoneIter().tryGetStringByKey(&mZoneId, "Id");
    return info.getPlacementIter().tryGetStringByKey(&mId, "Id");
}

bool PlacementId::isEqual(const PlacementId& otherId) const {
    if (mCommonId)
        return otherId.getCommonId() && isEqualString(mCommonId, otherId.getCommonId());

    if (otherId.getCommonId())
        return false;

    if (mUnitConfigName) {
        return otherId.getUnitConfigName() &&
               isEqualString(mUnitConfigName, otherId.getUnitConfigName()) &&
               isEqualString(mZoneId, otherId.getZoneId()) && mId && otherId.getId() &&
               isEqualString(mId, otherId.getId());
    }

    if (otherId.getUnitConfigName())
        return false;

    return mId && otherId.getId() && isEqualString(mId, otherId.getId());
}

bool PlacementId::isEqual(const PlacementId& selfId, const PlacementId& otherId) {
    if (selfId.getCommonId())
        return otherId.getCommonId() && isEqualString(selfId.getCommonId(), otherId.getCommonId());

    if (otherId.getCommonId())
        return false;

    if (selfId.getUnitConfigName()) {
        return otherId.getUnitConfigName() &&
               isEqualString(selfId.getUnitConfigName(), otherId.getUnitConfigName()) &&
               isEqualString(selfId.getZoneId(), otherId.getZoneId()) && selfId.getId() &&
               otherId.getId() && isEqualString(selfId.getId(), otherId.getId());
    }

    if (otherId.getUnitConfigName())
        return false;

    return selfId.getId() && otherId.getId() && isEqualString(selfId.getId(), otherId.getId());
}

bool PlacementId::isValid() const {
    return mCommonId || mId;
}

void PlacementId::makeString(sead::BufferedSafeString* out) const {
    if (mCommonId)
        out->format(mCommonId);
    else if (mUnitConfigName)
        out->format("%s(%s[%s])", mId, mUnitConfigName, mZoneId);
    else if (mId)
        out->format(mId);
    else
        out->format("");
}

StringTmp<128> makeStringPlacementId(const PlacementId* placementId) {
    StringTmp<128> string;
    if (placementId)
        placementId->makeString(&string);
    return string;
}

}  // namespace al
