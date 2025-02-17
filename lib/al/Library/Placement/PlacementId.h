#pragma once

#include <prim/seadSafeString.h>

namespace al {
class PlacementInfo;

class PlacementId {
public:
    static bool isEqual(const PlacementId&, const PlacementId&);

    PlacementId();
    PlacementId(const char*, const char*, const char*);

    bool init(const PlacementInfo&);
    bool isEqual(const PlacementId&) const;
    bool isValid() const;
    bool makeString(sead::BufferedSafeString*) const;

    const char* getId() const { return mId; }

    const char* getUnitConfigName() const { return mUnitConfigName; }

private:
    const char* mId;
    const char* mUnitConfigName;
    const char* mZoneId;
    const char* mCommonID;
};
}  // namespace al
