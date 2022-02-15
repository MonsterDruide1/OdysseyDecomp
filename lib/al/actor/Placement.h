#pragma once

#include <resource/seadResource.h>
#include "al/byaml/ByamlIter.h"

namespace al {
class PlacementInfo {
public:
    PlacementInfo();

    void set(const al::ByamlIter& placement_iter, const al::ByamlIter& zone_iter);

    const al::ByamlIter& getPlacementIter() const { return mPlacementIter; }
    const al::ByamlIter& getZoneIter() const { return mZoneIter; }

private:
    al::ByamlIter mPlacementIter;
    al::ByamlIter mZoneIter;
};

class PlacementId {
public:
    static bool isEqual(const al::PlacementId&, const al::PlacementId&);

    PlacementId();
    PlacementId(const char*, const char*, const char*);

    bool init(const al::PlacementInfo&);
    bool isEqual(const al::PlacementId&) const;
    bool isValid() const;
    bool makeString(sead::BufferedSafeString*) const;

private:
    const char* mId;
    const char* mUnitConfigName;
    const char* mZoneId;
    const char* mCommonID;
};
}  // namespace al
