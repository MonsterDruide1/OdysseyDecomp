#include "Player/PlayerJudgeDeadWipeStart.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerFunction.h"

PlayerJudgeDeadWipeStart::PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor,
                                                   const PlayerConst* playerConst)
    : mPlayerActor(playerActor), mPlayerConst(playerConst) {}

void PlayerJudgeDeadWipeStart::reset() {
    mDeathType = DeathType::Damage;
    mCounter = -1;
}

void PlayerJudgeDeadWipeStart::update() {
    if (PlayerFunction::isPlayerDeadStatus(mPlayerActor))
        mCounter++;
}

bool PlayerJudgeDeadWipeStart::judge() const {
    switch (mDeathType) {
    case DeathType::Damage:
        return mCounter >= mPlayerConst->getDeadWipeStartDamage();
    case DeathType::Abyss:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyss();
    case DeathType::AbyssWithCapMsg:
        return mCounter >= mPlayerConst->getDeadWipeStartAbyssWithCapMsg();
    case DeathType::Press:
        return mCounter >= mPlayerConst->getDeadWipeStartPress();
    case DeathType::SandSink:
        return mCounter >= mPlayerConst->getDeadWipeStartSandSink();
    case DeathType::NoOxygen:
        return mCounter >= mPlayerConst->getDeadWipeStartNoOxygen();
    case DeathType::IceWater:
        return mCounter >= mPlayerConst->getDeadWipeStartIceWater();
    default:
        return false;
    }
}

bool PlayerJudgeDeadWipeStart::isDeadTypeDrawForward() const {
    return mDeathType == DeathType::Damage || mDeathType == DeathType::Press ||
           mDeathType == DeathType::SandSink || mDeathType == DeathType::NoOxygen ||
           mDeathType == DeathType::IceWater;
}

// Determines if a ring of coins should appear for certain death types
bool PlayerJudgeDeadWipeStart::isDeadEnableCoinAppear() const {
    return mDeathType != DeathType::Press;
}

void PlayerJudgeDeadWipeStart::getWipeInfo(const char** name, s32* wait) const {
    *name = "WipeMiss";

    switch (mDeathType) {
    case DeathType::Damage:
        *wait = mPlayerConst->getDeadWipeWaitDamage();
        break;
    case DeathType::Abyss:
        *wait = mPlayerConst->getDeadWipeWaitAbyss();
        break;
    case DeathType::AbyssWithCapMsg:
        *wait = mPlayerConst->getDeadWipeWaitAbyssWithCapMsg();
        break;
    case DeathType::Press:
        *wait = mPlayerConst->getDeadWipeWaitPress();
        break;
    case DeathType::SandSink:
        *wait = mPlayerConst->getDeadWipeWaitSandSink();
        break;
    case DeathType::NoOxygen:
        *wait = mPlayerConst->getDeadWipeWaitNoOxygen();
        break;
    case DeathType::IceWater:
        *wait = mPlayerConst->getDeadWipeWaitIceWater();
        break;
    }
}
