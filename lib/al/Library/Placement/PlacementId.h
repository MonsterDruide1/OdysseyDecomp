#pragma once

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"

namespace al {
template <s32>
class StringTmp;

class PlacementInfo;

class PlacementId {
public:
    static bool isEqual(const PlacementId& selfId, const PlacementId& otherId);

    PlacementId();
    PlacementId(const char* id, const char* unitConfig, const char* zoneId);

    bool init(const PlacementInfo& info);
    bool isEqual(const PlacementId& otherId) const;
    bool isValid() const;
    void makeString(sead::BufferedSafeString* out) const;

    const char* getId() const { return mId; }

    const char* getUnitConfigName() const { return mUnitConfigName; }

    const char* getZoneId() const { return mZoneId; }

    const char* getCommonId() const { return mCommonId; }

private:
    const char* mId = nullptr;
    const char* mUnitConfigName = nullptr;
    const char* mZoneId = nullptr;
    const char* mCommonId = nullptr;
};

StringTmp<128> makeStringPlacementId(const PlacementId* placementId);

}  // namespace al
