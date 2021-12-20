#include "al/actor/Placement.h"

namespace al {
PlacementInfo::PlacementInfo() = default;

void PlacementInfo::set(const al::ByamlIter& r_0, const al::ByamlIter& rZoneIter) {
    _0 = r_0;
    mZoneIter = rZoneIter;
}

PlacementId::PlacementId()
    : mId(nullptr), mUnitConfigName(nullptr), mZoneId(nullptr), mCommonID(nullptr) {}

PlacementId::PlacementId(const char* p_0, const char* pUnitConfig, const char* pID)
    : mId(p_0), mUnitConfigName(pUnitConfig), mZoneId(pID), mCommonID(nullptr) {}

bool PlacementId::init(const al::PlacementInfo& rInfo) {
    mId = nullptr, mZoneId = nullptr, mUnitConfigName = nullptr, mCommonID = nullptr;

    rInfo._0.tryGetStringByKey(&mCommonID, "CommonId");
    rInfo.mZoneIter.tryGetStringByKey(&mUnitConfigName, "UnitConfigName");
    rInfo.mZoneIter.tryGetStringByKey(&mZoneId, "Id");
    return rInfo._0.tryGetStringByKey(&mId, "Id");
}

bool PlacementId::isValid() const {
    return mCommonID || mId;
}
};  // namespace al