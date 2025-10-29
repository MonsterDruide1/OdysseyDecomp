#pragma once

#include <math/seadVector.h>

namespace al {
class PlacementInfo;
class GravityPoint;

class GravityHolder {
public:
    GravityHolder();
    void init();
    void createGravity(const PlacementInfo& info);
    bool tryCalcGravity(sead::Vector3f* gravity, const sead::Vector3f& position) const;
    bool tryCalcBlendGravity(sead::Vector3f* gravity, const sead::Vector3f& position) const;

private:
    GravityPoint** mPoints = nullptr;
    s32 mCount = 0;
    s32 mSize = 256;
};

static_assert(sizeof(GravityHolder) == 0x10);

}  // namespace al
