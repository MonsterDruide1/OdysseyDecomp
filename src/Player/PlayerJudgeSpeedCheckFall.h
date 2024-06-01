#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;

class PlayerJudgeSpeedCheckFall : public al::HioNode, public IJudge {
public:
    PlayerJudgeSpeedCheckFall(const al::LiveActor* player, const IUsePlayerCollision* collision,
                              const PlayerConst* pConst, const IJudge* judgeWaterSurfaceRun);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollision;
    const PlayerConst* mConst;
    const IJudge* mJudgeStartWaterSurfaceRun;
    s32 mFramesNoCollideGround = 0;
};
