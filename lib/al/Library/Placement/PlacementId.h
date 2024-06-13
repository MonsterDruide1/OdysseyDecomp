#pragma once

#include <resource/seadResource.h>

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

private:
    const char* mId;
    const char* mUnitConfigName;
    const char* mZoneId;
    const char* mCommonID;
};
}  // namespace al
