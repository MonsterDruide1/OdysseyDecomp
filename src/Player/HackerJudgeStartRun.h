#pragma once

#include <basis/seadTypes.h>

#include "Player/HackerJudge.h"

namespace al {
class LiveActor;
}

class IUsePlayerCollision;
class IUsePlayerHack;

class HackerJudgeStartRun : public HackerJudge {
public:
    HackerJudgeStartRun(const al::LiveActor*, IUsePlayerHack**);
    void reset() override;
    void update() override;
    bool judge() const override;

    void setPlayerCollision(IUsePlayerCollision* playerCollision) {
        mPlayerCollision = playerCollision;
    }

private:
    al::LiveActor* mParent;
    s32 field_18;
    IUsePlayerCollision* mPlayerCollision;
    f32 field_28;
};
