#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class PlacementInfo;

class KeyPose {
public:
    KeyPose();

    void init(const PlacementInfo& info);

    const sead::Quatf& getQuat() const { return mQuat; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    const PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    PlacementInfo* mPlacementInfo = nullptr;
};

static_assert(sizeof(KeyPose) == 0x28);
}  // namespace al
