#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class BgmPlayObj;
class CameraTicket;
class EventFlowExecutor;
class HitSensor;
class JointLookAtController;
class JointRippleGenerator;
class SensorMsg;
class Nerve;
}  // namespace al

class DoshiStateWanderBossBattle;
class ShoppingWatcher;
class TalkNpcCap;

class Doshi : public al::LiveActor {
public:
    Doshi(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void onBattleStart();
    void doReaction(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void requestUpdateAction(f32, f32);
    bool isInWater() const;
    void requestStartCamera();
    void requestEndCamera();

    void exeWait();
    void exeRun();
    void exeReaction();
    void exeBossBattle();

private:
    ShoppingWatcher* mShoppingWatcher = nullptr;
    al::BgmPlayObj* mBgmPlayObj = nullptr;
    al::JointLookAtController* mJointLookAtController = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    al::Nerve* mActiveNerve = nullptr;
    DoshiStateWanderBossBattle* mWanderBossBattle = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    f32 mClippingRadius = 4500.0f;
    f32 mLocalOffsetY = 2500.0f;
    s32 mRemainingCoins = 5;
    al::CameraTicket* mCamera = nullptr;
    TalkNpcCap* mTalkNpcCap = nullptr;
    f32 mActionWaitFrame = -1;
    f32 mActionMoveFrame = -1;
    al::JointRippleGenerator* mJointRippleGeneratorNeck = nullptr;
    al::JointRippleGenerator* mJointRippleGeneratorSpine = nullptr;
    al::JointRippleGenerator* mJointRippleGeneratorTail = nullptr;
};
