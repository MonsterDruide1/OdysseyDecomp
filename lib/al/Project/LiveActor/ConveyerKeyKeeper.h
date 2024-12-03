#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class PlacementInfo;

struct ConveyerKey {
    void* _0[5];
    PlacementInfo& mPlacementInfo;
};

class ConveyerKeyKeeper {
public:
    ConveyerKeyKeeper();

    void init(const ActorInitInfo& info);
    void calcPosAndQuat(sead::Vector3f*, sead::Quatf*, s32*, f32) const;
    void calcPosAndQuatByKeyIndex(sead::Vector3f*, sead::Quatf*, s32 index) const;
    void calcClippingSphere(sead::Vector3f*, f32*, f32) const;
    const ConveyerKey* getConveyerKey(s32 index) const;  // return type depends on the type of _0

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
