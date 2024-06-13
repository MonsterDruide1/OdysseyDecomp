#pragma once

#include <math/seadVector.h>

namespace al {
class PlacementInfo;

class GravityHolder {
public:
    GravityHolder();
    void init();
    void createGravity(const PlacementInfo&);
    bool tryCalcGravity(sead::Vector3f*, const sead::Vector3f&) const;
    bool tryCalcBlendGravity(sead::Vector3f*, const sead::Vector3f&) const;

private:
    void* filler[2];
};
static_assert(sizeof(GravityHolder) == 0x10);

}  // namespace al
