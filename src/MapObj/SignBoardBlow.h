#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class SignBoardBlow : public al::LiveActor {
public:
    SignBoardBlow(const char* actorName, const char* signBoardBlowName);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startBlow(const sead::Vector3f&);
    void exeWait();
    void exeBlow();

private:
    const char* mName = nullptr;
    sead::Vector3f _110;
};

static_assert(sizeof(SignBoardBlow) == 0x120);
