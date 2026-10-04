#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class GiantWanderBossMine : public al::LiveActor {
public:
    GiantWanderBossMine(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appearAttach(const sead::Matrix34f*, const sead::Vector3f*);
    bool isLaunched() const;
    bool isEnableLaunch() const;
    bool isAttach() const;
    void startLaunchForOnGlass(const sead::Vector3f&);
    void startLaunchForFirstPhase();
    void startLaunchForEscape();
    void startLaunchForLongRange();
    void exeAppearAttach();
    void exeFlyDown();
    void resetPositionByAnim();
    void checkCollideAndSendMsg();
    void exeFlyParabolic();
    void exeSignExplosion();
    void exeExplosion();
    void exeDie();

private:
    void* padding[29];
};

static_assert(sizeof(GiantWanderBossMine) == 0x1f0);
