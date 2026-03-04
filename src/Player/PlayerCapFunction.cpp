#include "Player/PlayerCapFunction.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCap.h"
#include "Player/PlayerAnimator.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

void CapFunction::putOnCapPlayer(HackCap* hackCap, PlayerAnimator* animator) {
    hackCap->forcePutOn();
    animator->forceCapOn();
}

void CapFunction::endHack(HackCap* hackCap, PlayerAnimator* animator) {
    hackCap->endHack();
    animator->forceCapOn();
}

const HackObjInfo& CapFunction::getHackObjInfo(HackCap* hackCap) {
    const char* hackName = hackCap->mCapTargetInfo->mHackName;
    return *GameDataFunction::getHackObjInfo(GameDataHolderAccessor(hackCap), hackName);
}
