#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Player/IJudge.h"
#include "Player/PlayerConst.h"

class PlayerJudgeDeadWipeStart : public IJudge {
public:
    PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor, const PlayerConst* playerConst);
    void reset() override;
    void update() override;
    bool judge() const override;

    bool isDeadTypeDrawForward() const;
    bool isDeadEnableCoinAppear() const;
    void getWipeInfo(const char** outString, s32* wipeInfoOut) const;

private:
    const al::LiveActor* mPlayerActor;
    const PlayerConst* mPlayerConst;
    u32 mDeadType;
    s32 mCounter;
};
