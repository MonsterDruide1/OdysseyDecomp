#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class PlacementInfo;

struct ConveyerKey {
    f32 moveDistance;
    f32 totalMoveDistance;
    sead::Quatf quat;
    sead::Vector3f moveDistanceVertical;
    s32 interpolateType;
    const PlacementInfo* placementInfo;

    void init(const sead::Vector3f& keeperTrans, const sead::Vector3f& keeperDir,
              const PlacementInfo& placement);
};

class ConveyerKeyKeeper {
public:
    ConveyerKeyKeeper();

    void init(const ActorInitInfo& info);
    void calcPosAndQuat(sead::Vector3f* pos, sead::Quatf* quat, s32* index, f32 coord) const;
    void calcPosAndQuatByKeyIndex(sead::Vector3f* pos, sead::Quatf* quat, s32 index) const;
    void calcClippingSphere(sead::Vector3f* clippingTrans, f32* clippingRadius, f32 offset) const;
    const ConveyerKey& getConveyerKey(s32 index) const;

    s32 getConveyerKeyCount() const { return mConveyerKeyCount; }

    f32 getTotalMoveDistance() const { return mTotalMoveDistance; }

private:
    ConveyerKey* mConveyerKeys = nullptr;
    s32 mConveyerKeyCount = 0;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mMoveDirection = sead::Vector3f::ez;
    f32 mTotalMoveDistance = 0.0f;
};

static_assert(sizeof(ConveyerKeyKeeper) == 0x38);

}  // namespace al
