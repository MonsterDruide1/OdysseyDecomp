#pragma once

#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class ConveyerKeyKeeper;
class PlacementInfo;

struct ConveyerKey {
    void* _0[5];
    PlacementInfo* mPlacementInfo;
};

class ConveyerKeyKeeper {
public:
    ConveyerKeyKeeper();

    void init(const ActorInitInfo& info);
    void calcPosAndQuat(sead::Vector3f*, sead::Quatf*, s32*, f32) const;
    void calcPosAndQuatByKeyIndex(sead::Vector3f*, sead::Quatf*, s32 index) const;
    void calcClippingSphere(sead::Vector3f*, f32*, f32) const;
    const ConveyerKey& getConveyerKey(s32 index) const;  // return type depends on the type of _0

    f32 get_34() const { return _34; }

private:
    ConveyerKey* mConveyerKeys;  // array of a struct/class with a size of 0x30?
    s32 mConveyerKeyCount;
    sead::Quatf mQuat;
    sead::Vector3f mTrans;
    sead::Vector3f _28;
    f32 _34;
};

class ConveyerStep : public LiveActor {
public:
    ConveyerStep(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void setHost(LiveActor* host);
    void setConveyerKeyKeeper(const ConveyerKeyKeeper* conveyerKeyKeeper, f32);
    void setTransByCoord(f32, bool);
    void setTransByCoord(f32, bool, bool) __attribute__((noinline));
    void setTransAndResetByCoord(f32);

    void exeWait();

    const ConveyerKeyKeeper* getConveyerKeyKeeper() const { return mConveyerKeyKeeper; }

    bool get_130() const { return _130; }

    void set_128(float ukn) { _128 = ukn; }

    void set_130(bool ukn) { _130 = ukn; }

private:
    LiveActor* mHost = nullptr;
    const ConveyerKeyKeeper* mConveyerKeyKeeper = nullptr;
    const char* mKeyHitReactionName = nullptr;
    const char* mActionName = nullptr;
    f32 _128 = 0.0f;
    f32 _12c = 0.0f;
    bool _130 = true;
};
}  // namespace al
