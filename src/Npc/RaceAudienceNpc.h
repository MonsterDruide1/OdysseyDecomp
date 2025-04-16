#pragma once

#include "Library/LiveActor/LiveActor.h"

class RaceAudienceNpc : public al::LiveActor {
public:
    enum class ActionType { Wait, Dance, Jump };

    RaceAudienceNpc(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeDance();
    void exeDanceRandom();
    void exeJump();

private:
    ActionType mAudienceActionType = ActionType::Wait;
    s32 mDanceTimer = 0;
};
