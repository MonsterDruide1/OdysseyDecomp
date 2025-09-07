#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class CapHanger;
class SignBoardBlow;

class SignBoardDanger : public al::LiveActor {
public:
    SignBoardDanger(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) override;
    bool isCanStartReaction();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void exeWait();
    void exeReaction();
    void exeDead();

private:
    SignBoardBlow* mSignBoardBlow = nullptr;
    f32 mYRotation = 0.0f;
    s32 mRespawnTimer = 0;
    CapHanger* mCapHanger = nullptr;
};

static_assert(sizeof(SignBoardDanger) == 0x120);
