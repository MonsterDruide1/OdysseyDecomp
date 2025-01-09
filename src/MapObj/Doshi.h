#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class BgmPlayObj;
class EventFlowExecutor;
class HitSensor;
class JointLookAtController;
class JointRippleGenerator;
class SensorMsg;
class Nerve;
}  // namespace al

class DoshiStateWanderBossBattle;
class ShoppingWatcher;

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
    void isInWater() const;
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
    al::Nerve* mNerve = nullptr;
    DoshiStateWanderBossBattle* mWanderBossBattle = nullptr;
    sead::Vector3f mOffset = sead::Vector3f::zero;
    sead::Vector3f mClipping = sead::Vector3f(4500.0f, 2500.0f, 0.0f);
    al::LiveActor* mShoppingNpc = nullptr;
    f32 mActionA = -1;
    f32 mActionB = -1;
    al::JointRippleGenerator* mJointRippleGeneratorA = nullptr;
    al::JointRippleGenerator* mJointRippleGeneratorB = nullptr;
    al::JointRippleGenerator* mJointRippleGeneratorC = nullptr;
};
