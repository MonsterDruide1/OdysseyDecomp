#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class IUsePlayerCollision;

class HackerJudgeNormalFall : public al::HioNode, public IJudge {
public:
    HackerJudgeNormalFall(const al::LiveActor* parent, s32 unk);
    void reset() override;
    void update() override;
    bool judge() const override;

    void setPlayerCollision(IUsePlayerCollision* playerCollision) {
        mPlayerCollision = playerCollision;
    }

private:
    al::LiveActor* mActor;
    s32 _10;
    s32 _14;
    IUsePlayerCollision* mPlayerCollision;
};

static_assert(sizeof(HackerJudgeNormalFall) == 0x20);
