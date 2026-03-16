#include "Player/PlayerCapFunction.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCap.h"
#include "Player/PlayerAnimator.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

namespace CapFunction {
void putOnCapPlayer(HackCap* hackCap, PlayerAnimator* animator) {
    hackCap->forcePutOn();
    animator->forceCapOn();
}

void endHack(HackCap* hackCap, PlayerAnimator* animator) {
    hackCap->endHack();
    animator->forceCapOn();
}

const HackObjInfo* getHackObjInfo(HackCap* hackCap) {
    const char* hackName = hackCap->getCapTargetInfo()->getHackName();
    return GameDataFunction::getHackObjInfo(GameDataHolderAccessor(hackCap), hackName);
}
}  // namespace CapFunction
