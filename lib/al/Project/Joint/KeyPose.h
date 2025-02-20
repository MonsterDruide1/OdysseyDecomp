#pragma once

#include <math/seadQuat.h>

namespace al {
class PlacementInfo;

class KeyPose {
public:
    KeyPose();

    void init(const PlacementInfo& info);

    const sead::Quatf& getQuat() { return mQuat; }

    const sead::Vector3f& getTrans() { return mTrans; }

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    PlacementInfo* mPlacementInfo = nullptr;
};

static_assert(sizeof(KeyPose) == 0x28);
}  // namespace al
