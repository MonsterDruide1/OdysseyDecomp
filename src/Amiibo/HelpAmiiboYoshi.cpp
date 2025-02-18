#include "Amiibo/HelpAmiiboYoshi.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nfp/NfpFunction.h"

#include "Player/Yoshi.h"
#include "Util/PlayerUtil.h"

HelpAmiiboYoshi::HelpAmiiboYoshi(HelpAmiiboDirector* director, al::LiveActor* amiiboActor)
    : HelpAmiiboExecutor(director, amiiboActor, "ヨッシーお助け") {}

void HelpAmiiboYoshi::initAfterPlacement(const al::ActorInitInfo& actorInitInfo) {
    HelpAmiiboExecutor::initAfterPlacement(actorInitInfo);

    mYoshi = new Yoshi("amiiboヨッシー");
    al::initCreateActorNoPlacementInfo(mYoshi, actorInitInfo);
    mYoshi->makeActorDead();
}

bool HelpAmiiboYoshi::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseYoshi(nfpInfo);
}

bool HelpAmiiboYoshi::execute() {
    return !al::isAlive(mYoshi);
}

void HelpAmiiboYoshi::activate() {
    HelpAmiiboExecutor::activate();

    const sead::Vector3f& playerPos = rs::getPlayerPos(getActor());
    al::resetPosition(mYoshi, playerPos + sead::Vector3f(0, 1000.0f, 0));
    mYoshi->appearEgg();
}

HelpAmiiboType HelpAmiiboYoshi::getType() const {
    return HelpAmiiboType::Yoshi;
}
