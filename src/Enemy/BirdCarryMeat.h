#pragma once

#include "Library/LiveActor/LiveActor.h"

class BalloonIcon;
class CarryMeat;

namespace al {
class Rail;
class RailRider;
}  // namespace al

class BirdCarryMeat : public al::LiveActor {
public:
    BirdCarryMeat(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    sead::Matrix34f* getBindMtx() const;
    void skipDemo();
    void exeWaitOnRail();
    void exeDemoCarryMeat();
    void exeMoveMeat();
    void exeReactionCarry();
    void exeDrop();
    void exeFlyAway();

    bool isDropNerve() const;
    al::RailRider* getRailRider() const override;

private:
    CarryMeat* mCarryMeat = nullptr;
    al::RailKeeper* mWaitRailKeeper = nullptr;
    al::RailKeeper* mRailKeeper = nullptr;
    BalloonIcon* mBalloonIcon = nullptr;
    u64 _128 = 0;
    u32 _130 = 0;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Quatf mInitQuat = sead::Quatf::unit;
    sead::Matrix34f mJointMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(BirdCarryMeat) == 0x180);
