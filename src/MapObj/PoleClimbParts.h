#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/MapObj/BreakMapPartsBase.h"

#include "Util/BreakJudgeUtil.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class PoleClimbParts : public al::LiveActor {
public:
    PoleClimbParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeBreak();
    void exeReaction();

private:
    bool mIsBreak = false;
    al::JudgeFuncPtr mBreakJudgeFunction = nullptr;
};

static_assert(sizeof(PoleClimbParts) == 0x118);
