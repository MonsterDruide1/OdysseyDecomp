#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EventFlowExecutor;
class JointAimInfo;
}  // namespace al

class BossRaidNpc : public al::LiveActor {
public:
    BossRaidNpc(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    bool isEnableReaction() const;
    void exeEvent();
    void exeReaction();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    al::JointAimInfo* mJointAimInfoL = nullptr;
    al::JointAimInfo* mJointAimInfoR = nullptr;
};

static_assert(sizeof(BossRaidNpc) == 0x120);
