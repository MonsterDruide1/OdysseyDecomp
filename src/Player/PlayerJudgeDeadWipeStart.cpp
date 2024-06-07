#include "Player/PlayerJudgeDeadWipeStart.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerFunction.h"

PlayerJudgeDeadWipeStart::PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor,
                                                   const PlayerConst* playerConst)
    : mPlayerActor(playerActor), mPlayerConst(playerConst) {}

void PlayerJudgeDeadWipeStart::reset() {
    mDeathType = Damage;
    mCounter = -1;
}

void PlayerJudgeDeadWipeStart::update() {
    if (PlayerFunction::isPlayerDeadStatus(mPlayerActor))
        mCounter++;
}

bool PlayerJudgeDeadWipeStart::judge() const {
    switch (mDeathType) {
    case Damage:
        return mCounter >= mPlayerConst->getDeadWipeStartDamage();
    case Abyss:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyss();
    case AbyssWithCapMsg:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyssWithCapMsg();
    case Press:
        return mCounter >= mPlayerConst->getDeadWipeStartPress();
    case SandSink:
        return mCounter >= mPlayerConst->getDeadWipeStartSandSink();
    case NoOxygen:
        return mCounter >= mPlayerConst->getDeadWipeStartNoOxygen();
    case IceWater:
        return mCounter >= mPlayerConst->getDeadWipeStartIceWater();
    default:
        return false;
    }
}

bool PlayerJudgeDeadWipeStart::isDeadTypeDrawForward() const {
    return mDeathType == Damage || mDeathType == Press || mDeathType == SandSink ||
           mDeathType == NoOxygen || mDeathType == IceWater;
}

bool PlayerJudgeDeadWipeStart::isDeadEnableCoinAppear() const {
    return mDeathType != Press;
}

void PlayerJudgeDeadWipeStart::getWipeInfo(const char** name, s32* wait) const {
    *name = "WipeMiss";

    switch (mDeathType) {
    case Damage:
        *wait = mPlayerConst->getDeadWipeWaitDamage();
        break;
    case Abyss:
        *wait = mPlayerConst->getDeadWipeWaitAbyss();
        break;
    case AbyssWithCapMsg:
        *wait = mPlayerConst->getDeadWipeWaitAbyssWithCapMsg();
        break;
    case Press:
        *wait = mPlayerConst->getDeadWipeWaitPress();
        break;
    case SandSink:
        *wait = mPlayerConst->getDeadWipeWaitSandSink();
        break;
    case NoOxygen:
        *wait = mPlayerConst->getDeadWipeWaitNoOxygen();
        break;
    case IceWater:
        *wait = mPlayerConst->getDeadWipeWaitIceWater();
        break;
    }
}
