#include "Player/PlayerJudgeDeadWipeStart.h"

PlayerJudgeDeadWipeStart::PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor,
                                                   const PlayerConst* playerConst)
    : mPlayerActor(playerActor), mPlayerConst(playerConst), mFlags(0xFFFFFFFF00000000LL) {}
