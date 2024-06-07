#include "Player/PlayerJudgeDeadWipeStart.h"

#include "Player/PlayerFunction.h"

PlayerJudgeDeadWipeStart::PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor,
                                                   const PlayerConst* playerConst)
    : mPlayerActor(playerActor), mPlayerConst(playerConst), mDeadType(0), mCounter(-1) {}

void PlayerJudgeDeadWipeStart::reset() {
    mDeadType = 0;
    mCounter = -1;
}

void PlayerJudgeDeadWipeStart::update() {
    if (PlayerFunction::isPlayerDeadStatus(mPlayerActor))
        mCounter++;
}

bool PlayerJudgeDeadWipeStart::judge() const {
    switch (mDeadType) {
    case 0:
        return mCounter >= mPlayerConst->getDeadWipeStartDamage();
        break;
    case 1:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyss();
        break;
    case 2:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyssWithCapMsg();
        break;
    case 3:
        return mCounter >= mPlayerConst->getDeadWipeStartPress();
        break;
    case 4:
        return mCounter >= mPlayerConst->getDeadWipeStartSandSink();
        break;
    case 5:
        return mCounter >= mPlayerConst->getDeadWipeStartNoOxygen();
        break;
    case 6:
        return mCounter >= mPlayerConst->getDeadWipeStartIceWater();
        break;
    default:
        return false;
    }
}

bool PlayerJudgeDeadWipeStart::isDeadTypeDrawForward() const {
    return mDeadType == 0 || mDeadType == 3 || mDeadType == 4 || mDeadType == 5 || mDeadType == 6;
}

bool PlayerJudgeDeadWipeStart::isDeadEnableCoinAppear() const {
    return mDeadType != 3;
}

void PlayerJudgeDeadWipeStart::getWipeInfo(const char** outString, s32* wipeInfoOut) const {
    *outString = "WipeMiss";

    switch (mDeadType) {
    case 0:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitDamage();
        break;
    case 1:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitAbyss();
        break;
    case 2:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitAbyssWithCapMsg();
        break;
    case 3:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitPress();
        break;
    case 4:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitSandSink();
        break;
    case 5:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitNoOxygen();
        break;
    case 6:
        *wipeInfoOut = mPlayerConst->getDeadWipeWaitIceWater();
        break;
    }
}
