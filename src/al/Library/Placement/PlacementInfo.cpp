#include "al/Library/Placement/Placement.h"

#include "al/Library/Yaml/ByamlIter.h"

namespace al {
PlacementInfo::PlacementInfo() = default;

void PlacementInfo::set(const al::ByamlIter& placement_iter, const al::ByamlIter& zone_iter) {
    mPlacementIter = placement_iter;
    mZoneIter = zone_iter;
}

PlacementId::PlacementId()
    : mId(nullptr), mUnitConfigName(nullptr), mZoneId(nullptr), mCommonID(nullptr) {}

PlacementId::PlacementId(const char* p_0, const char* pUnitConfig, const char* pID)
    : mId(p_0), mUnitConfigName(pUnitConfig), mZoneId(pID), mCommonID(nullptr) {}

bool PlacementId::init(const al::PlacementInfo& rInfo) {
    mId = nullptr, mZoneId = nullptr, mUnitConfigName = nullptr, mCommonID = nullptr;

    rInfo.getPlacementIter().tryGetStringByKey(&mCommonID, "CommonId");
    rInfo.getZoneIter().tryGetStringByKey(&mUnitConfigName, "UnitConfigName");
    rInfo.getZoneIter().tryGetStringByKey(&mZoneId, "Id");
    return rInfo.getPlacementIter().tryGetStringByKey(&mId, "Id");
}

bool PlacementId::isValid() const {
    return mCommonID || mId;
}
};  // namespace al
