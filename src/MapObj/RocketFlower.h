#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class RocketFlower : public al::LiveActor {
public:
    RocketFlower(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    bool isEnableEquip() const;
    void terminateFollow();

    void exeWait();
    void exeWaitFollow();
    bool trySyncFlyingCapPos();
    void exeFollow();
    void appearFlowerSub();
    void exeWaitAttach();
    void exeAttach();

    void setFollowFlowerPose(const sead::Quatf& quat, const sead::Vector3f& trans);
    void disappear();
    void disappearForce();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    al::LiveActor* mFlowerSub = nullptr;
    s32 mReactionFrame = 0;
    s32 mFollowLostFrame = 0;
};

static_assert(sizeof(RocketFlower) == 0x120);
