#pragma once

#include <basis/seadTypes.h>

#include "Util/HackerJudge.h"

namespace al {
class LiveActor;
}

class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerCounterForceRun;

class HackerJudgeStartRun : public HackerJudge {
public:
    HackerJudgeStartRun(const al::LiveActor* parent, IUsePlayerHack** hacker);

    void reset() override {}

    void update() override {}

    bool judge() const override;

    void setPlayerCollision(IUsePlayerCollision* playerCollision) {
        mPlayerCollision = playerCollision;
    }

private:
    const al::LiveActor* mParent;
    const PlayerCounterForceRun* mCounterForceRun = nullptr;
    IUsePlayerCollision* mPlayerCollision = nullptr;
    f32 mAutoRunSpeedThreshold = 0.0f;
};

static_assert(sizeof(HackerJudgeStartRun) == 0x30);
