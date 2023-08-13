#include "Library/Placement/PlacementId.h"

#include "Library/Placement/PlacementInfo.h"

namespace al {
PlacementId::PlacementId()
    : mId(nullptr), mUnitConfigName(nullptr), mZoneId(nullptr), mCommonID(nullptr) {}

PlacementId::PlacementId(const char* p_0, const char* pUnitConfig, const char* pID)
    : mId(p_0), mUnitConfigName(pUnitConfig), mZoneId(pID), mCommonID(nullptr) {}

bool PlacementId::init(const PlacementInfo& rInfo) {
    mId = nullptr, mZoneId = nullptr, mUnitConfigName = nullptr, mCommonID = nullptr;

    rInfo.getPlacementIter().tryGetStringByKey(&mCommonID, "CommonId");
    rInfo.getZoneIter().tryGetStringByKey(&mUnitConfigName, "UnitConfigName");
    rInfo.getZoneIter().tryGetStringByKey(&mZoneId, "Id");
    return rInfo.getPlacementIter().tryGetStringByKey(&mId, "Id");
}

bool PlacementId::isValid() const {
    return mCommonID || mId;
}
}  // namespace al
