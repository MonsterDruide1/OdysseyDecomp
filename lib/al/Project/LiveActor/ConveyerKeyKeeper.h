#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class PlacementInfo;

// TODO: I'm not sure about this
struct ConveyerKey {
    void* _0[5];
    PlacementInfo& mPlacementInfo;
};

class ConveyerKeyKeeper {
public:
    ConveyerKeyKeeper();

    void init(const ActorInitInfo& info);
    void calcPosAndQuat(sead::Vector3f* pos, sead::Quatf* quat, s32* index, f32 coord) const;
    void calcPosAndQuatByKeyIndex(sead::Vector3f* pos, sead::Quatf* quat, s32 index) const;
    void calcClippingSphere(sead::Vector3f* clippingTrans, f32* clippingRadius, f32 offset) const;
    const ConveyerKey* getConveyerKey(s32 index) const;  // return type depends on the type of _0

    s32 getConveyerKeyCount() const { return mConveyerKeyCount; }

    f32 get_34() const { return _34; }

private:
    ConveyerKey** mConveyerKeys;  // array of a struct/class with a size of 0x30?
    s32 mConveyerKeyCount;
    sead::Quatf mQuat;
    sead::Vector3f mTrans;
    sead::Vector3f _28;
    f32 _34;
};
}  // namespace al
