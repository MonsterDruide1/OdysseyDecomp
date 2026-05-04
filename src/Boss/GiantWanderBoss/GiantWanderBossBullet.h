#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class GiantWanderBossBullet : public al::LiveActor {
public:
    GiantWanderBossBullet(const char*);

    void init(const al::ActorInitInfo&) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appearAttach(const sead::Matrix34f*, const sead::Vector3f*, bool);
    void startLaunch();
    bool isLaunched() const;
    void exeAppearAttach();
    void exeFly();
    void resetPositionByAnim();
    void checkCollideAndSendMsg();

private:
    u8 _108[0x78];
};

static_assert(sizeof(GiantWanderBossBullet) == 0x180);
