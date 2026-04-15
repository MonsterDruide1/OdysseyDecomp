#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class SignBoard : public al::LiveActor {
public:
    SignBoard(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeReaction();

private:
    bool mIsWall = false;
};

static_assert(sizeof(SignBoard) == 0x110);
